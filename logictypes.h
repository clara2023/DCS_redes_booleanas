#ifndef LOGICTYPES_H
#define LOGICTYPES_H
#pragma once

#include <QString>
#include <QList>
#include <QStringList>

enum class InteractionMode {
    Standard,
    Linking,
    Excluding
};

enum class LogicFunction {
    AND,
    OR,
    NOT,
    XOR,
    NAND,
    NOR,
    EQUAL
    //CUSTOM
};

inline QString toQString(LogicFunction f) {
    switch (f) {
        case LogicFunction::AND: return "AND";
        case LogicFunction::OR: return "OR";
        case LogicFunction::NOT: return "NOT";
        case LogicFunction::XOR: return "XOR";
        case LogicFunction::NAND: return "NAND";
        case LogicFunction::NOR: return "NOR";
        case LogicFunction::EQUAL: return "EQUAL";

        default: return "UNKNOWN";
    }
}

inline QList<LogicFunction> allLogicFunctions() {
    return {
        LogicFunction::AND,
        LogicFunction::OR,
        LogicFunction::NOT,
        LogicFunction::XOR,
        LogicFunction::NAND,
        LogicFunction::NOR,
        LogicFunction::EQUAL
    };
}

inline QStringList allLogicFunctionNames() {
    QStringList list;
    for (LogicFunction func : allLogicFunctions()) {
        list << toQString(func);
    }
    return list;
}


#endif // LOGICTYPES_H
