#include <extortion/extortion.hpp>

#include "utilities.hpp"
#include "expression.hpp"

#include <expat.h>

#include <list>
#include <stack>
#include <boost/scope_exit.hpp>
#include <boost/variant/apply_visitor.hpp>

namespace extortion { namespace detail {

class callback_visitor : public boost::static_visitor<> {
    active_set &recorders;
    unsigned depth;

public:
    callback_visitor(
        active_set &recorders_,
        unsigned depth_
    ) :
        recorders(recorders_),
        depth(depth_)
    {}

    void operator()(
        const std::tr1::function<void(const std::string&)> &cb
    ) const {
        recorders.add(depth, cb);
    }

    void operator()(
        const std::tr1::function<void(const char*, size_t, bool)> &cb
    ) const {
        recorders.add(depth, cb);
    }
};

} // namespace extortion::detail

class extortion::impl {
    XML_Parser parser;

    std::list<detail::expression> expressions;
    typedef std::vector<detail::expression*> expression_set;
    std::stack<expression_set> exs;
    detail::active_set recorders;

public:
    impl() :
        parser(XML_ParserCreate(0))
    {
        XML_SetElementHandler(
            parser,
            &extortion::impl::start_element_proxy,
            &extortion::impl::end_element_proxy
        );

        XML_SetCharacterDataHandler(
            parser,
            &extortion::impl::cdata_proxy
        );

        XML_SetUserData(parser, this);
    }

    ~impl() {
        XML_ParserFree(parser);
    }

    void start_element(const XML_Char *name, const XML_Char **atts) {
        recorders << '<' << name << atts << '>';
        expression_set::const_iterator first(exs.top().begin());
        expression_set::const_iterator last(exs.top().end());

        exs.push(expression_set());
        std::vector<detail::expression*> &next_generation = exs.top();

        for (; first!=last; ++first) {
            detail::component &comp = (*first)->components[exs.size()-2];
            if (comp.match(name, atts)) {
                // End of expression and everything matched, start recording.
                if (exs.size()-1 == (*first)->components.size()) {
                    boost::apply_visitor(
                        detail::callback_visitor(recorders, exs.size()-1),
                        (*first)->cb
                    );
                } else {
                    next_generation.push_back(*first);
                }
            }
        }
    }

    void end_element(const XML_Char *name) {
        exs.pop();
        recorders.clear(exs.size());
        recorders << '<' << '/' << name << '>';
    }

    void cdata(const XML_Char *s, int len) {
        recorders.write(s, len);
    }

    bool parse(const char *data, size_t len, bool isfinal) {
        return XML_Parse(parser, &data[0], len, isfinal) != 0;
    }

    template<typename T>
    bool monitor_expression(
        const std::string &path,
        T cb
    ) {
        expressions.push_back(detail::expression());
        bool commit(false);

        BOOST_SCOPE_EXIT_TPL((&commit)(&expressions)(&exs)) {
            if(!commit) {
                expressions.pop_back();
            } else {
                if (exs.empty()) {
                    exs.push(expression_set());
                }
                exs.top().push_back(&(expressions.back()));
            }
        } BOOST_SCOPE_EXIT_END;

        detail::expression &nex = expressions.back();
        nex.cb = cb;
        commit = ::extortion::detail::parse(path, nex);
        return commit;
    }

    static void start_element_proxy(
        void *userdata,
        const XML_Char *name,
        const XML_Char **atts
    ) {
        reinterpret_cast<extortion::impl*>(userdata)->start_element(name, atts);
    }

    static void end_element_proxy(
        void *userdata,
        const XML_Char *name
    ) {
        reinterpret_cast<extortion::impl*>(userdata)->end_element(name);
    }

    static void cdata_proxy(
        void *userdata,
        const XML_Char *s,
        int len
    ) {
        reinterpret_cast<extortion::impl*>(userdata)->cdata(s, len);
    }
};



extortion::extortion()
    : pimpl(new impl)
{}

extortion::~extortion() {
    delete pimpl;
}

bool extortion::get(
    const std::string &path,
    const std::tr1::function<void(const std::string&)> &cb
) {
    return pimpl->monitor_expression(path, cb);
}

bool extortion::stream(
    const std::string &path,
    const std::tr1::function<void(const char*, size_t, bool)> &cb
) {
    return pimpl->monitor_expression(path, cb);
}

bool extortion::parse(const char *data, size_t len) {
    return pimpl->parse(data, len, len == 0);
}

bool extortion::parse_string(const std::string &data) {
    return pimpl->parse(data.data(), data.size(), true);
}

}
