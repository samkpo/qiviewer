#ifndef MNAME
#define MNAME
#include <QString>

//Returns class::method
// inline std::string methodName(const std::string& prettyFunction)
// {
//     size_t colons = prettyFunction.find("::");
//     size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
//     size_t end = prettyFunction.rfind("(") - begin;

//     return prettyFunction.substr(begin,end) + "()";
// }
// #define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__).c_str()
inline QString methodName(const QString prettyFunction)
{
    size_t colons = prettyFunction.indexOf("::");
    size_t begin = prettyFunction.mid(0,colons).indexOf(" ") + 1;
    size_t end = prettyFunction.lastIndexOf("(") - begin;

    return prettyFunction.mid(begin,end) + "()";
}
#define __METHOD_NAME__ methodName(__PRETTY_FUNCTION__).toLatin1().constData()

#endif