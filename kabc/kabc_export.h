/*
  This file is part of the kabc library.
  Copyright (c) 2006-2007 Allen Winter <winter@kde.org>

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

#ifndef KABC_EXPORT_H
#define KABC_EXPORT_H

#include <kdemacros.h>

#ifndef KABC_EXPORT
# if defined(MAKE_KABC_LIB)
   /* We are building this library */
#  define KABC_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define KABC_EXPORT KDE_IMPORT
# endif
#endif

#ifndef KABC_FILE_CORE_EXPORT
# if defined(MAKE_KABC_FILE_CORE_LIB)
   /* We are building this library */
#  define KABC_FILE_CORE_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define KABC_FILE_CORE_EXPORT KDE_IMPORT
# endif
#endif

#ifndef KABC_DIR_EXPORT
# if defined(MAKE_KABC_DIRECTORY_LIB)
   /* We are building this library */
#  define KABC_DIR_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define KABC_DIR_EXPORT KDE_IMPORT
# endif
#endif

#ifndef KABC_NET_EXPORT
# if defined(MAKE_KABC_NET_LIB)
   /* We are building this library */
#  define KABC_NET_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define KABC_NET_EXPORT KDE_IMPORT
# endif
#endif

#ifndef KABC_LDAPKIO_EXPORT
# if defined(MAKE_KABC_LDAPKIO_LIB)
   /* We are building this library */
#  define KABC_LDAPKIO_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define KABC_LDAPKIO_EXPORT KDE_IMPORT
# endif
#endif

# ifndef KABC_EXPORT_DEPRECATED
#  define KABC_EXPORT_DEPRECATED KDE_DEPRECATED KABC_EXPORT
# endif
# ifndef KABC_FILE_CORE_EXPORT_DEPRECATED
#  define KABC_FILE_CORE_EXPORT_DEPRECATED KDE_DEPRECATED KABC_FILE_CORE_EXPORT
# endif
# ifndef KABC_DIRECTORY_EXPORT_DEPRECATED
#  define KABC_DIRECTORY_EXPORT_DEPRECATED KDE_DEPRECATED KABC_DIRECTORY_EXPORT
# endif
# ifndef KABC_NET_EXPORT_DEPRECATED
#  define KABC_NET_EXPORT_DEPRECATED KDE_DEPRECATED KABC_NET_EXPORT
# endif
# ifndef KABC_LDAPKIO_EXPORT_DEPRECATED
#  define KABC_LDAPKIO_EXPORT_DEPRECATED KDE_DEPRECATED KABC_LDAPKIO_EXPORT
# endif

#endif
