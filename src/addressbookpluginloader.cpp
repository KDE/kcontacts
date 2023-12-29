/*
    SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addressbookpluginloader.h"

#include <QCoreApplication>
#include <QDirIterator>
#include <QPluginLoader>

using namespace KContacts;

struct PluginLoader {
    PluginLoader();
    std::vector<std::unique_ptr<KContacts::AddressBookPlugin>> plugins;
};

PluginLoader::PluginLoader()
{
    // static plugins
    const auto staticPluginData = QPluginLoader::staticPlugins();
    for (const auto &data : staticPluginData) {
        if (data.metaData().value(QLatin1String("IID")).toString() == QLatin1String("org.kde.kcontacts.AddressBookPlugin")) {
            plugins.push_back(std::unique_ptr<KContacts::AddressBookPlugin>(qobject_cast<KContacts::AddressBookPlugin *>(data.instance())));
        }
    }

    // dynamic plugins
    QStringList searchPaths(QCoreApplication::applicationDirPath());
    searchPaths += QCoreApplication::libraryPaths();

    for (const auto &searchPath : std::as_const(searchPaths)) {
        const QString pluginPath = searchPath + QLatin1String("/kf6/org.kde.kcontacts.addressbooks");
        for (QDirIterator it(pluginPath, QDir::Files); it.hasNext();) {
            it.next();
            QPluginLoader loader(it.fileInfo().absoluteFilePath());
            if (loader.load()) {
                plugins.push_back(std::unique_ptr<KContacts::AddressBookPlugin>{qobject_cast<KContacts::AddressBookPlugin *>(loader.instance())});
            } else {
                qDebug() << loader.errorString();
            }
        }
    }
}

Q_GLOBAL_STATIC(PluginLoader, s_pluginLoader)

bool AddressBookPluginLoader::hasPlugin()
{
    return (bool)s_pluginLoader->plugins.size() > 0;
}

QList<KContacts::AddressBookPlugin *> AddressBookPluginLoader::plugins()
{
    QList<KContacts::AddressBookPlugin *> plugins;
    for (const auto &plugin : s_pluginLoader->plugins) {
        plugins << plugin.get();
    }
    return plugins;
}

#include "moc_addressbookpluginloader.cpp"
