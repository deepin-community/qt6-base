// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QTest>
#include <QtCore/QScopedPointer>

/*!
 \class tst_QScopedPointer
 \internal
 \since 4.6
 \brief Tests class QScopedPointer.

 */
class tst_QScopedPointer : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void defaultConstructor();
    void dataOnDefaultConstructed();
    void useSubClassInConstructor();
    void dataOnValue();
    void dataSignature();
    void reset();
    void dereferenceOperator();
    void dereferenceOperatorSignature();
    void pointerOperator();
    void pointerOperatorSignature();
    void negationOperator();
    void negationOperatorSignature();
    void operatorBool();
    void operatorBoolSignature();
    void isNull();
    void isNullSignature();
    void objectSize();
    void comparison();
    void array();
    // TODO instanciate on const object
};

void tst_QScopedPointer::defaultConstructor()
{
    /* Check that the members, one, is correctly initialized. */
    QScopedPointer<int> p;
    QCOMPARE(p.data(), static_cast<int *>(0));
    QCOMPARE(p.get(), static_cast<int *>(0));
}

void tst_QScopedPointer::dataOnDefaultConstructed()
{
    QScopedPointer<int> p;

    QCOMPARE(p.data(), static_cast<int *>(0));
    QCOMPARE(p.get(), static_cast<int *>(0));
}

class MyClass
{
};

class MySubClass : public MyClass
{
};

void tst_QScopedPointer::useSubClassInConstructor()
{
    /* Use a syntax which users typically would do. */
    QScopedPointer<MyClass> p(new MySubClass());
}

void tst_QScopedPointer::dataOnValue()
{
    int *const rawPointer = new int(5);
    QScopedPointer<int> p(rawPointer);

    QCOMPARE(p.data(), rawPointer);
}

void tst_QScopedPointer::dataSignature()
{
    const QScopedPointer<int> p;
    /* data() should be const. */
    p.data();
}

void tst_QScopedPointer::reset()
{
    /* Call reset() on a default constructed value. */
    {
        QScopedPointer<int> p;
        p.reset();
        QCOMPARE(p.data(), static_cast<int *>(0));
        QCOMPARE(p.get(), static_cast<int *>(0));
    }

    /* Call reset() on an active value. */
    {
        QScopedPointer<int> p(new int(3));
        p.reset();
        QCOMPARE(p.data(), static_cast<int *>(0));
        QCOMPARE(p.get(), static_cast<int *>(0));
    }

    /* Call reset() with a value, on an active value. */
    {
        QScopedPointer<int> p(new int(3));

        int *const value = new int(9);
        p.reset(value);
        QCOMPARE(*p.data(), 9);
        QCOMPARE(*p.get(), 9);
    }

    /* Call reset() with a value, on default constructed value. */
    {
        QScopedPointer<int> p;

        int *const value = new int(9);
        p.reset(value);
        QCOMPARE(*p.data(), 9);
        QCOMPARE(*p.get(), 9);
    }
}

class AbstractClass
{
public:
    virtual ~AbstractClass()
    {
    }

    virtual int member() const = 0;
};

class SubClass : public AbstractClass
{
public:
    virtual int member() const override
    {
        return 5;
    }
};

void tst_QScopedPointer::dereferenceOperator()
{
    /* Dereference a basic value. */
    {
        QScopedPointer<int> p(new int(5));

        const int value2 = *p;
        QCOMPARE(value2, 5);
    }

    /* Dereference a pointer to an abstract class. This verifies
     * that the operator returns a reference, when compiling
     * with MSVC 2005. */
    {
        QScopedPointer<AbstractClass> p(new SubClass());

        QCOMPARE((*p).member(), 5);
    }
}

void tst_QScopedPointer::dereferenceOperatorSignature()
{
    /* The operator should be const. */
    {
        const QScopedPointer<int> p(new int(5));
        *p;
    }

    /* A reference should be returned, not a value. */
    {
        const QScopedPointer<int> p(new int(5));
        Q_UNUSED(static_cast<int &>(*p));
    }

    /* Instantiated on a const object, the returned object is a const reference. */
    {
        const QScopedPointer<const int> p(new int(5));
        Q_UNUSED(static_cast<const int &>(*p));
    }
}

class AnyForm
{
public:
    int value;
};

void tst_QScopedPointer::pointerOperator()
{
    QScopedPointer<AnyForm> p(new AnyForm());
    p->value = 5;

    QCOMPARE(p->value, 5);
}

void tst_QScopedPointer::pointerOperatorSignature()
{
    /* The operator should be const. */
    const QScopedPointer<AnyForm> p(new AnyForm);
    p->value = 5;

    QVERIFY(p->value);
}

void tst_QScopedPointer::negationOperator()
{
    /* Invoke on default constructed value. */
    {
        QScopedPointer<int> p;
        QVERIFY(!p);
    }

    /* Invoke on a value. */
    {
        QScopedPointer<int> p(new int(2));
        QCOMPARE(!p, false);
    }
}

void tst_QScopedPointer::negationOperatorSignature()
{
    /* The signature should be const. */
    const QScopedPointer<int> p;
    !p;

    /* The return value should be bool. */
    Q_UNUSED(static_cast<bool>(!p));
}

void tst_QScopedPointer::operatorBool()
{
    /* Invoke on default constructed value. */
    {
        QScopedPointer<int> p;
        QCOMPARE(bool(p), false);
    }

    /* Invoke on active value. */
    {
        QScopedPointer<int> p(new int(3));
        QVERIFY(p);
    }
}

void tst_QScopedPointer::operatorBoolSignature()
{
    /* The signature should be const and return bool. */
    const QScopedPointer<int> p;

    (void)static_cast<bool>(p);
}

void tst_QScopedPointer::isNull()
{
    /* Invoke on default constructed value. */
    {
        QScopedPointer<int> p;
        QVERIFY(p.isNull());
        QVERIFY(p == nullptr);
        QVERIFY(nullptr == p);
    }

    /* Invoke on a set value. */
    {
        QScopedPointer<int> p(new int(69));
        QVERIFY(!p.isNull());
        QVERIFY(p != nullptr);
        QVERIFY(nullptr != p);
    }
}

void tst_QScopedPointer::isNullSignature()
{
    const QScopedPointer<int> p(new int(69));

    /* The signature should be const and return bool. */
    Q_UNUSED(static_cast<bool>(p.isNull()));
}

void tst_QScopedPointer::objectSize()
{
    /* The size of QScopedPointer should be the same as one pointer. */
    QCOMPARE(sizeof(QScopedPointer<int>), sizeof(void *));
}

struct RefCounted
{
    RefCounted()
        : ref(0)
    {
        instanceCount.ref();
    }

    RefCounted(RefCounted const &)
        : ref(0)
    {
        instanceCount.ref();
    }

    ~RefCounted()
    {
        QVERIFY( ref.loadRelaxed() == 0 );
        instanceCount.deref();
    }

    RefCounted &operator=(RefCounted const &)
    {
        return *this;
    }

    QAtomicInt ref;

    static QAtomicInt instanceCount;
};

QAtomicInt RefCounted::instanceCount = 0;

template <class A1, class A2, class B>
void scopedPointerComparisonTest(const A1 &a1, const A2 &a2, const B &b)
{
    // test equality on equal pointers
    QVERIFY(a1 == a2);
    QVERIFY(a2 == a1);

    // test inequality on equal pointers
    QVERIFY(!(a1 != a2));
    QVERIFY(!(a2 != a1));

    // test equality on unequal pointers
    QVERIFY(!(a1 == b));
    QVERIFY(!(a2 == b));
    QVERIFY(!(b == a1));
    QVERIFY(!(b == a2));

    // test inequality on unequal pointers
    QVERIFY(b != a1);
    QVERIFY(b != a2);
    QVERIFY(a1 != b);
    QVERIFY(a2 != b);
}

void tst_QScopedPointer::comparison()
{
    QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 0 );

    {
        RefCounted *a = new RefCounted;
        RefCounted *b = new RefCounted;

        QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 2 );

        QScopedPointer<RefCounted> pa1(a);
        QScopedPointer<RefCounted> pa2(a);
        QScopedPointer<RefCounted> pb(b);

        scopedPointerComparisonTest(pa1, pa1, pb);
        scopedPointerComparisonTest(pa2, pa2, pb);
        scopedPointerComparisonTest(pa1, pa2, pb);

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
        pa2.take();
QT_WARNING_POP

        QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 2 );
    }

    QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 0 );

    {
        RefCounted *a = new RefCounted[42];
        RefCounted *b = new RefCounted[43];

        QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 85 );

        QScopedArrayPointer<RefCounted> pa1(a);
        QScopedArrayPointer<RefCounted> pa2(a);
        QScopedArrayPointer<RefCounted> pb(b);

        scopedPointerComparisonTest(pa1, pa2, pb);

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
        pa2.take();
QT_WARNING_POP

        QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 85 );
    }

    QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 0 );

    {
        RefCounted *a = new RefCounted;
        RefCounted *b = new RefCounted;

        QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 2 );

        QSharedDataPointer<RefCounted> pa1(a);
        QSharedDataPointer<RefCounted> pa2(a);
        QSharedDataPointer<RefCounted> pb(b);

        QCOMPARE( a->ref.loadRelaxed(), 2 );
        QCOMPARE( b->ref.loadRelaxed(), 1 );
        QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 2 );

        scopedPointerComparisonTest(pa1, pa2, pb);

        QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 2 );
    }

    QCOMPARE( RefCounted::instanceCount.loadRelaxed(), 0 );
}

void tst_QScopedPointer::array()
{
    int instCount = RefCounted::instanceCount.loadRelaxed();
    {
        QScopedArrayPointer<RefCounted> array;
        array.reset(new RefCounted[42]);
        QCOMPARE(instCount + 42, RefCounted::instanceCount.loadRelaxed());
    }
    QCOMPARE(instCount, RefCounted::instanceCount.loadRelaxed());
    {
        QScopedArrayPointer<RefCounted> array(new RefCounted[42]);
        QCOMPARE(instCount + 42, RefCounted::instanceCount.loadRelaxed());
        array.reset(new RefCounted[28]);
        QCOMPARE(instCount + 28, RefCounted::instanceCount.loadRelaxed());
        array.reset(0);
        QCOMPARE(instCount, RefCounted::instanceCount.loadRelaxed());
    }
    QCOMPARE(instCount, RefCounted::instanceCount.loadRelaxed());
}


QTEST_MAIN(tst_QScopedPointer)
#include "tst_qscopedpointer.moc"
