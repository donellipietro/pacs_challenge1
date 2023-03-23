#ifndef __MUPARSER_FUN__

#define __MUPARSER_FUN__

#include <muParser.h>

#include <memory>
#include <string>
class MuparserFun
{
public:
    MuparserFun(const MuparserFun &m)
        : m_parser(m.m_parser)
    {
        m_parser.DefineVar("t", &t_);
        m_parser.DefineVar("y", &y_);
    };

    MuparserFun(const std::string &s)
    {
        try
        {
            m_parser.DefineVar("t", &t_);
            m_parser.DefineVar("y", &y_);
            m_parser.SetExpr(s);
        }
        catch (mu::Parser::exception_type &e)
        {
            std::cerr << e.GetMsg() << std::endl;
        }
    };

    double
    operator()(const double &t, const double &y = 0)
    {
        t_ = t;
        y_ = y;
        double result = 0.;
        try
        {
            result = m_parser.Eval();
        }
        catch (mu::Parser::exception_type &e)
        {
            std::cerr << e.GetMsg() << std::endl;
        }
        return result;
    };

private:
    double t_;
    double y_;
    mu::Parser m_parser;
};

#endif // __MUPARSER_FUN__