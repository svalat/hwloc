# Copyright © 2015 Inria.  All rights reserved.
# See COPYING in top-level directory.

AM_CFLAGS = $(HWLOC_CFLAGS)
AM_CPPFLAGS = $(HWLOC_CPPFLAGS)
AM_LDFLAGS = $(HWLOC_LDFLAGS)

# Add your expected output file here.
# By default, it shows the output of `lstopo --of xml -'.
cpuid_outputs = \
	Intel-Haswell-2xXeon-E5-2680v3.output \
	Intel-IvyBridge-12xXeon-E5-4620v2.output \
	Intel-SandyBridge-2xXeon-E5-2650.output \
	Intel-Westmere-2xXeon-X5650.output \
	Intel-Nehalem-2xXeon-X5550.output \
	Intel-Penryn-4xXeon-X7460.output \
	Intel-Core-2xXeon-E5345.output \
	Intel-KnightsCorner-XeonPhi-SE10P.output \
	AMD-15h-Bulldozer-4xOpteron-6272.output \
	AMD-K10-MagnyCours-2xOpteron-6164HE.output \
	AMD-K10-Istanbul-8xOpteron-8439SE.output \
	AMD-K8-SantaRosa-2xOpteron-2218.output \
	AMD-K8-SledgeHammer-2xOpteron-250.output

# FIXME add AMD-K10-Shanghai/Barcelona-[248]3xx
# FIXME add AMD-15h-Piledriver-[46]3xx

# Each output `xyz.output' must have a corresponding tarball `xyz.tar.bz2'
# specifying which tarball to use
cpuid_tarballs = \
	Intel-Haswell-2xXeon-E5-2680v3.tar.bz2 \
	Intel-IvyBridge-12xXeon-E5-4620v2.tar.bz2 \
	Intel-SandyBridge-2xXeon-E5-2650.tar.bz2 \
	Intel-Westmere-2xXeon-X5650.tar.bz2 \
	Intel-Nehalem-2xXeon-X5550.tar.bz2 \
	Intel-Penryn-4xXeon-X7460.tar.bz2 \
	Intel-Core-2xXeon-E5345.tar.bz2 \
	Intel-KnightsCorner-XeonPhi-SE10P.tar.bz2 \
	AMD-15h-Bulldozer-4xOpteron-6272.tar.bz2 \
	AMD-K10-MagnyCours-2xOpteron-6164HE.tar.bz2 \
	AMD-K10-Istanbul-8xOpteron-8439SE.tar.bz2 \
	AMD-K8-SantaRosa-2xOpteron-2218.tar.bz2 \
	AMD-K8-SledgeHammer-2xOpteron-250.tar.bz2

# Each output `xyz.output' may have a corresponding `xyz.options'
# file modifying the behavior of lstopo
cpuid_options = 

# Each output `xyz.output' may have a corresponding `xyz.env'
# modifying the environment of lstopo
cpuid_envs = 

if HWLOC_HAVE_BUNZIPP
TESTS = $(cpuid_outputs)
endif HWLOC_HAVE_BUNZIPP

EXTRA_DIST = $(cpuid_outputs) $(cpuid_tarballs) $(cpuid_options) $(cpuid_envs)

LOG_COMPILER = $(builddir)/test-topology.sh
