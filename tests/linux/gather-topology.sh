#!/bin/sh
#-*-sh-*-

#
# Copyright © 2009 CNRS, INRIA, Université Bordeaux 1
#
# This software is a computer program whose purpose is to provide
# abstracted information about the hardware topology.
#
# This software is governed by the CeCILL-B license under French law and
# abiding by the rules of distribution of free software.  You can  use,
# modify and/ or redistribute the software under the terms of the CeCILL-B
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info".
#
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability.
#
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or
# data to be ensured and,  more generally, to use and operate it in the
# same conditions as regards security.
#
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL-B license and that you accept its terms.
#

name="$1"; shift
[ -z "$name" ] && echo "Save name needed as an argument" && exit -1

destdir=`mktemp -d`

# Get all files from the given path (either a file or a directory)
# ignore errors since some files may be missing, and some may be
# Restricted to root (but we don't need them).
# Use cat so that we properly get proc/sys files even if their
# file length is wrong
savepath() {
  local dest="$1"
  local path="$2"
  find "$path" -type f 2>/dev/null | while read file ; do	\
    mkdir -p "$dest/"`dirname $file` ;		\
    cat "$file" > "$dest/$file" 2>/dev/null ;	\
  done
}

# Gather the following list of files and directories
cat << EOF | while read path ; do savepath "$destdir/$name" "$path" ; done
/sys/devices/system/cpu/
/sys/devices/system/node/
/sys/class/dmi/id/
/proc/cpuinfo
/proc/meminfo
/proc/stat
EOF

# Create the archive and keep the tree in /tmp for testing
( cd "$destdir/" && tar cfz "$name.tar.gz" "$name" )
mv "$destdir/$name.tar.gz" "./$name.tar.gz"
echo "Hierarchy gathered in ./$name.tar.gz and kept in $destdir/$name/"

exit 0
