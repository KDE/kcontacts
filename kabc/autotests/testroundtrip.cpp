/*
    This file is part of libkabc.
    Copyright (c) 2012 Kevin Krammer <krammer@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "addressee.h"
#include "vcardconverter.h"

#include <qtest.h>
#include <QDir>
#include <QObject>

using namespace KABC;

class RoundtripTest : public QObject
{
    Q_OBJECT

private:
    QString mOutFilePattern;

    QDir mInputDir;
    QDir mOutput2_1Dir;
    QDir mOutput3_0Dir;
    QDir mOutput4_0Dir;

    QStringList mInputFiles;

private Q_SLOTS:
    void initTestCase();
    void testVCardRoundtrip_data();
    void testVCardRoundtrip();
};

// check the validity of our test data set
void RoundtripTest::initTestCase()
{
    mOutFilePattern = QLatin1String("%1.ref");

    // check that all resource prefixes exist

    mInputDir = QDir(QLatin1String(":/input"));
    QVERIFY(mInputDir.exists());
    QVERIFY(mInputDir.cd(QLatin1String("data")));

    mOutput2_1Dir = QDir(QLatin1String(":/output2.1"));
    QVERIFY(mOutput2_1Dir.exists());
    QVERIFY(mOutput2_1Dir.cd(QLatin1String("data")));

    mOutput3_0Dir = QDir(QLatin1String(":/output3.0"));
    QVERIFY(mOutput3_0Dir.exists());
    QVERIFY(mOutput3_0Dir.cd(QLatin1String("data")));

    mOutput4_0Dir = QDir(QLatin1String(":/output4.0"));
    QVERIFY(mOutput4_0Dir.exists());
    QVERIFY(mOutput4_0Dir.cd(QLatin1String("data")));

    // check that there are input files

    mInputFiles = mInputDir.entryList();
    QVERIFY(!mInputFiles.isEmpty());
}

void RoundtripTest::testVCardRoundtrip_data()
{
    QTest::addColumn<QString>("inputFile");
    QTest::addColumn<QString>("output2_1File");
    QTest::addColumn<QString>("output3_0File");
    QTest::addColumn<QString>("output4_0File");

    Q_FOREACH (const QString &inputFile, mInputFiles) {
        const QString outFile = mOutFilePattern.arg(inputFile);

        QTest::newRow(QFile::encodeName(inputFile).constData())
                << inputFile
                << (mOutput2_1Dir.exists(outFile) ? outFile : QString())
                << (mOutput3_0Dir.exists(outFile) ? outFile : QString())
                << (mOutput4_0Dir.exists(outFile) ? outFile : QString());
    }
}

void RoundtripTest::testVCardRoundtrip()
{
    QFETCH(QString, inputFile);
    QFETCH(QString, output2_1File);
    QFETCH(QString, output3_0File);
    QFETCH(QString, output4_0File);

    QVERIFY2(!output2_1File.isEmpty()
             || !output3_0File.isEmpty()
             || !output4_0File.isEmpty(),
             "No reference output file for either format version");

    QFile input(QFileInfo(mInputDir, inputFile).absoluteFilePath());
    QVERIFY(input.open(QIODevice::ReadOnly));

    const QByteArray inputData = input.readAll();
    QVERIFY(!inputData.isEmpty());

    VCardConverter converter;
    const Addressee::List list = converter.parseVCards(inputData);
    QVERIFY(!list.isEmpty());

    if (!output2_1File.isEmpty()) {
        const QByteArray outputData = converter.createVCards(list, VCardConverter::v2_1);

        QFile outputFile(QFileInfo(mOutput2_1Dir, output2_1File).absoluteFilePath());
        QVERIFY(outputFile.open(QIODevice::ReadOnly));

        const QByteArray outputRefData = outputFile.readAll();
        QCOMPARE(outputData.size(), outputRefData.size());

        const QList<QByteArray> outputLines = outputData.split('\n');
        const QList<QByteArray> outputRefLines = outputRefData.split('\n');
        QCOMPARE(outputLines.count(), outputRefLines.count());

        for (int i = 0; i < outputLines.count(); ++i) {
            const QByteArray actual = outputLines[ i ];
            const QByteArray expect = outputRefLines[ i ];

            if (actual != expect) {
                qCritical() << "Mismatch in v2.1 output line" << (i + 1);
                QCOMPARE(actual.count(), expect.count());

                qCritical() << "\nActual:" << actual << "\nExpect:" << expect;
                QCOMPARE(actual, expect);
            }
        }
    }

    if (!output3_0File.isEmpty()) {
        const QByteArray outputData = converter.createVCards(list, VCardConverter::v3_0);

        QFile outputFile(QFileInfo(mOutput3_0Dir, output3_0File).absoluteFilePath());
        QVERIFY(outputFile.open(QIODevice::ReadOnly));

        const QByteArray outputRefData = outputFile.readAll();
//    QCOMPARE( outputData.size(), outputRefData.size() );

        const QList<QByteArray> outputLines = outputData.split('\n');
        const QList<QByteArray> outputRefLines = outputRefData.split('\n');
        QCOMPARE(outputLines.count(), outputRefLines.count());

        for (int i = 0; i < outputLines.count(); ++i) {
            const QByteArray actual = outputLines[ i ];
            const QByteArray expect = outputRefLines[ i ];

            if (actual != expect) {
                qCritical() << "Mismatch in v3.0 output line" << (i + 1);

                qCritical() << "\nActual:" << actual << "\nExpect:" << expect;
                QCOMPARE(actual.count(), expect.count());
                QCOMPARE(actual, expect);
            }
        }
    }
}

QTEST_MAIN(RoundtripTest)

#include "testroundtrip.moc"
