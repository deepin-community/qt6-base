// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <private/qmimetype_p.h>

#include <qmimetype.h>
#include <qmimedatabase.h>
#include <QVariantMap>

#include <QTest>


class tst_qmimetype : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void isValid();
    void name();
    void genericIconName();
    void iconName();
    void suffixes();
    void gadget();
};

// ------------------------------------------------------------------------------------------------

void tst_qmimetype::initTestCase()
{
    qputenv("XDG_DATA_DIRS", "doesnotexist");
}

// ------------------------------------------------------------------------------------------------

static QString qMimeTypeName()
{
    static const QString result ("No name of the MIME type");
    return result;
}

static QString qMimeTypeGenericIconName()
{
    static const QString result ("No file name of an icon image that represents the MIME type");
    return result;
}

static QString qMimeTypeIconName()
{
    static const QString result ("No file name of an icon image that represents the MIME type");
    return result;
}

static QStringList buildQMimeTypeFilenameExtensions()
{
    QStringList result;
    result << QString::fromLatin1("*.png");
    return result;
}

static QStringList qMimeTypeGlobPatterns()
{
    static const QStringList result (buildQMimeTypeFilenameExtensions());
    return result;
}

// ------------------------------------------------------------------------------------------------

QMIMETYPE_BUILDER_FROM_RVALUE_REFS

// ------------------------------------------------------------------------------------------------

void tst_qmimetype::isValid()
{
    QMimeType instantiatedQMimeType (
                  buildQMimeType (
                      qMimeTypeName(),
                      qMimeTypeGenericIconName(),
                      qMimeTypeIconName(),
                      qMimeTypeGlobPatterns()
                  )
              );

    QVERIFY(instantiatedQMimeType.isValid());

    QMimeType otherQMimeType (instantiatedQMimeType);

    QVERIFY(otherQMimeType.isValid());
    QCOMPARE(instantiatedQMimeType, otherQMimeType);

    QMimeType defaultQMimeType;

    QVERIFY(!defaultQMimeType.isValid());
}

// ------------------------------------------------------------------------------------------------

void tst_qmimetype::name()
{
    QMimeType instantiatedQMimeType (
                  buildQMimeType (
                      qMimeTypeName(),
                      qMimeTypeGenericIconName(),
                      qMimeTypeIconName(),
                      qMimeTypeGlobPatterns()
                  )
              );

    QMimeType otherQMimeType (
                  buildQMimeType (
                      QString(),
                      qMimeTypeGenericIconName(),
                      qMimeTypeIconName(),
                      qMimeTypeGlobPatterns()
                  )
              );

    // Verify that the Name is part of the equality test:
    QCOMPARE(instantiatedQMimeType.name(), qMimeTypeName());

    QVERIFY(instantiatedQMimeType != otherQMimeType);
    QVERIFY(!(instantiatedQMimeType == otherQMimeType));
}

// ------------------------------------------------------------------------------------------------

void tst_qmimetype::genericIconName()
{
    QMimeType instantiatedQMimeType (
                  buildQMimeType (
                      qMimeTypeName(),
                      qMimeTypeGenericIconName(),
                      qMimeTypeIconName(),
                      qMimeTypeGlobPatterns()
                  )
              );

    QCOMPARE(instantiatedQMimeType.genericIconName(), qMimeTypeGenericIconName());
}

// ------------------------------------------------------------------------------------------------

void tst_qmimetype::iconName()
{
    QMimeType instantiatedQMimeType (
                  buildQMimeType (
                      qMimeTypeName(),
                      qMimeTypeGenericIconName(),
                      qMimeTypeIconName(),
                      qMimeTypeGlobPatterns()
                  )
              );

    QCOMPARE(instantiatedQMimeType.iconName(), qMimeTypeIconName());
}

// ------------------------------------------------------------------------------------------------

void tst_qmimetype::suffixes()
{
    QMimeType instantiatedQMimeType (
                  buildQMimeType (
                      qMimeTypeName(),
                      qMimeTypeGenericIconName(),
                      qMimeTypeIconName(),
                      qMimeTypeGlobPatterns()
                  )
              );

    QCOMPARE(instantiatedQMimeType.globPatterns(), qMimeTypeGlobPatterns());
    QCOMPARE(instantiatedQMimeType.suffixes(), QStringList() << QString::fromLatin1("png"));
}

// ------------------------------------------------------------------------------------------------

void tst_qmimetype::gadget()
{
    QMimeType instantiatedQMimeType (
                  buildQMimeType (
                      qMimeTypeName(),
                      qMimeTypeGenericIconName(),
                      qMimeTypeIconName(),
                      qMimeTypeGlobPatterns()
                  )
              );

    const QMetaObject *metaObject = &instantiatedQMimeType.staticMetaObject;

    QCOMPARE(metaObject->className(), "QMimeType");
    QVariantMap properties;
    for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); i++) {
        QMetaProperty property = metaObject->property(i);
        properties[property.name()] = property.readOnGadget(&instantiatedQMimeType);
    }

    QCOMPARE(properties["valid"].toBool(), instantiatedQMimeType.isValid());
    QCOMPARE(properties["isDefault"].toBool(), instantiatedQMimeType.isDefault());
    QCOMPARE(properties["name"].toString(), instantiatedQMimeType.name());
    QCOMPARE(properties["comment"].toString(), instantiatedQMimeType.comment());
    QCOMPARE(properties["genericIconName"].toString(), instantiatedQMimeType.genericIconName());
    QCOMPARE(properties["iconName"].toString(), instantiatedQMimeType.iconName());
    QCOMPARE(properties["globPatterns"].toStringList(), instantiatedQMimeType.globPatterns());
    QCOMPARE(properties["parentMimeTypes"].toStringList(), instantiatedQMimeType.parentMimeTypes());
    QCOMPARE(properties["allAncestors"].toStringList(), instantiatedQMimeType.allAncestors());
    QCOMPARE(properties["aliases"].toStringList(), instantiatedQMimeType.aliases());
    QCOMPARE(properties["suffixes"].toStringList(), instantiatedQMimeType.suffixes());
    QCOMPARE(properties["preferredSuffix"].toString(), instantiatedQMimeType.preferredSuffix());
    QCOMPARE(properties["filterString"].toString(), instantiatedQMimeType.filterString());

    QVERIFY(metaObject->indexOfMethod("inherits(QString)") >= 0);
}

// ------------------------------------------------------------------------------------------------

QTEST_GUILESS_MAIN(tst_qmimetype)
#include "tst_qmimetype.moc"
