/****************************************************************************
** Meta object code from reading C++ file 'mtplay.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mtplay.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mtplay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mtplay_t {
    QByteArrayData data[13];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mtplay_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mtplay_t qt_meta_stringdata_mtplay = {
    {
QT_MOC_LITERAL(0, 0, 6), // "mtplay"
QT_MOC_LITERAL(1, 7, 7), // "videoTd"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 7), // "audioTd"
QT_MOC_LITERAL(4, 24, 6), // "readTd"
QT_MOC_LITERAL(5, 31, 8), // "filename"
QT_MOC_LITERAL(6, 40, 8), // "openfile"
QT_MOC_LITERAL(7, 49, 7), // "isPause"
QT_MOC_LITERAL(8, 57, 13), // "ShowVideoTime"
QT_MOC_LITERAL(9, 71, 8), // "totalMs1"
QT_MOC_LITERAL(10, 80, 11), // "SliderPress"
QT_MOC_LITERAL(11, 92, 13), // "SliderRelease"
QT_MOC_LITERAL(12, 106, 7) // "myClose"

    },
    "mtplay\0videoTd\0\0audioTd\0readTd\0filename\0"
    "openfile\0isPause\0ShowVideoTime\0totalMs1\0"
    "SliderPress\0SliderRelease\0myClose"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mtplay[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    1,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   64,    2, 0x0a /* Public */,
       7,    0,   65,    2, 0x0a /* Public */,
       8,    1,   66,    2, 0x0a /* Public */,
      10,    0,   69,    2, 0x0a /* Public */,
      11,    0,   70,    2, 0x0a /* Public */,
      12,    0,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void mtplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        mtplay *_t = static_cast<mtplay *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->videoTd(); break;
        case 1: _t->audioTd(); break;
        case 2: _t->readTd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->openfile(); break;
        case 4: _t->isPause(); break;
        case 5: _t->ShowVideoTime((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->SliderPress(); break;
        case 7: _t->SliderRelease(); break;
        case 8: _t->myClose(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (mtplay::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mtplay::videoTd)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (mtplay::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mtplay::audioTd)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (mtplay::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&mtplay::readTd)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject mtplay::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_mtplay.data,
      qt_meta_data_mtplay,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *mtplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mtplay::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mtplay.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int mtplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void mtplay::videoTd()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void mtplay::audioTd()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void mtplay::readTd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
