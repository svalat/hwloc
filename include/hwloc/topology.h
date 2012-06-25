#ifndef HWLOC_TOPOLOGY_H
#define HWLOC_TOPOLOGY_H

#include <private/autogen/config.h>
#include <hwloc.h>
#include <hwloc/bitmap.h>
#include <private/debug.h>
#include <sys/types.h>
#include <hwloc/backend.h>

extern void hwloc_linuxfs_pci_lookup_osdevices(struct hwloc_topology *topology, struct hwloc_obj *pcidev);
extern int hwloc_linuxfs_get_pcidev_cpuset(struct hwloc_topology *topology, struct hwloc_obj *pcidev, hwloc_bitmap_t cpuset);

struct hwloc_used_backends{
	struct hwloc_backend_st* backend;
	struct hwloc_used_backends* next;
	struct hwloc_used_backends* previous;
};

enum hwloc_ignore_type_e {
  HWLOC_IGNORE_TYPE_NEVER = 0,
  HWLOC_IGNORE_TYPE_KEEP_STRUCTURE,
  HWLOC_IGNORE_TYPE_ALWAYS
};

#define HWLOC_DEPTH_MAX 128

struct hwloc_topology {
	unsigned nb_levels;					/* Number of horizontal levels */
	unsigned next_group_depth;				/* Depth of the next Group object that we may create */
	unsigned level_nbobjects[HWLOC_DEPTH_MAX]; 		/* Number of objects on each horizontal level */
	struct hwloc_obj **levels[HWLOC_DEPTH_MAX];		/* Direct access to levels, levels[l = 0 .. nblevels-1][0..level_nbobjects[l]] */
	unsigned long flags;
	int type_depth[HWLOC_OBJ_TYPE_MAX];
	enum hwloc_ignore_type_e ignored_types[HWLOC_OBJ_TYPE_MAX];
	int is_thissystem;
	int is_loaded;
	hwloc_pid_t pid;                                      /* Process ID the topology is view from, 0 for self */

	unsigned bridge_nbobjects;
	struct hwloc_obj **bridge_level;
	struct hwloc_obj *first_bridge, *last_bridge;
	unsigned pcidev_nbobjects;
	struct hwloc_obj **pcidev_level;
	struct hwloc_obj *first_pcidev, *last_pcidev;
	unsigned osdev_nbobjects;
	struct hwloc_obj **osdev_level;
	struct hwloc_obj *first_osdev, *last_osdev;

	int (*set_thisproc_cpubind)(hwloc_topology_t topology, hwloc_const_cpuset_t set, int flags);
	int (*get_thisproc_cpubind)(hwloc_topology_t topology, hwloc_cpuset_t set, int flags);
	int (*set_thisthread_cpubind)(hwloc_topology_t topology, hwloc_const_cpuset_t set, int flags);
	int (*get_thisthread_cpubind)(hwloc_topology_t topology, hwloc_cpuset_t set, int flags);
	int (*set_proc_cpubind)(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_const_cpuset_t set, int flags);
	int (*get_proc_cpubind)(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_cpuset_t set, int flags);
#ifdef hwloc_thread_t
	int (*set_thread_cpubind)(hwloc_topology_t topology, hwloc_thread_t tid, hwloc_const_cpuset_t set, int flags);
	int (*get_thread_cpubind)(hwloc_topology_t topology, hwloc_thread_t tid, hwloc_cpuset_t set, int flags);
#endif

	int (*get_thisproc_last_cpu_location)(hwloc_topology_t topology, hwloc_cpuset_t set, int flags);
	int (*get_thisthread_last_cpu_location)(hwloc_topology_t topology, hwloc_cpuset_t set, int flags);
	int (*get_proc_last_cpu_location)(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_cpuset_t set, int flags);

	int (*set_thisproc_membind)(hwloc_topology_t topology, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags);
	int (*get_thisproc_membind)(hwloc_topology_t topology, hwloc_nodeset_t nodeset, hwloc_membind_policy_t * policy, int flags);
	int (*set_thisthread_membind)(hwloc_topology_t topology, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags);
	int (*get_thisthread_membind)(hwloc_topology_t topology, hwloc_nodeset_t nodeset, hwloc_membind_policy_t * policy, int flags);
	int (*set_proc_membind)(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags);
	int (*get_proc_membind)(hwloc_topology_t topology, hwloc_pid_t pid, hwloc_nodeset_t nodeset, hwloc_membind_policy_t * policy, int flags);
	int (*set_area_membind)(hwloc_topology_t topology, const void *addr, size_t len, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags);
	int (*get_area_membind)(hwloc_topology_t topology, const void *addr, size_t len, hwloc_nodeset_t nodeset, hwloc_membind_policy_t * policy, int flags);
	/* This has to return the same kind of pointer as alloc_membind, so that free_membind can be used on it */
	void *(*alloc)(hwloc_topology_t topology, size_t len);
	/* alloc_membind has to always succeed if !(flags & HWLOC_MEMBIND_STRICT).
	 * see hwloc_alloc_or_fail which is convenient for that.  */
	void *(*alloc_membind)(hwloc_topology_t topology, size_t len, hwloc_const_nodeset_t nodeset, hwloc_membind_policy_t policy, int flags);
	int (*free_membind)(hwloc_topology_t topology, void *addr, size_t len);

	struct hwloc_topology_support support;

	struct hwloc_os_distances_s {
		hwloc_obj_type_t type;
		int nbobjs;
		unsigned *indexes; /* array of OS indexes before we can convert them into objs. always available.
		*/
		struct hwloc_obj **objs; /* array of objects, in the same order as above.
								  * either given (by a backend) together with the indexes array above.
								  * or build from the above indexes array when not given (by the user).
								  */
		float *distances; /* distance matrices, ordered according to the above indexes/objs array.
						   * distance from i to j is stored in slot i*nbnodes+j.
						   * will be copied into the main logical-index-ordered distance at the end of the discovery.
						   */
		int forced; /* set if the user forced a matrix to ignore the OS one */

		struct hwloc_os_distances_s *prev, *next;
	} *first_osdist, *last_osdist;

	/*hwloc_backend_t backend_type;*/
	struct hwloc_used_backends* used_backends;

/* N'existera plus car lié au backend */
	union hwloc_backend_params_u {
#ifdef HWLOC_LINUX_SYS
		struct hwloc_backend_params_linuxfs_s {
			/* FS root parameters */
			char *root_path; /* The path of the file system root, used when browsing, e.g., Linux' sysfs and procfs. */
			int root_fd; /* The file descriptor for the file system root, used when browsing, e.g., Linux' sysfs and procfs. */
			struct utsname utsname; /* cached result of uname, used multiple times */
		} linuxfs;
#endif /* HWLOC_LINUX_SYS */
#if defined(HWLOC_OSF_SYS) || defined(HWLOC_COMPILE_PORTS)
		struct hwloc_backend_params_osf {
			int nbnodes;
		} osf;
#endif /* HWLOC_OSF_SYS */
		struct hwloc_backend_params_xml_s {
			/* xml backend parameters */
#ifdef HWLOC_HAVE_LIBXML2
			void *doc;
#endif /* HWLOC_HAVE_LIBXML2 */
			char *buffer; /* only used when not using libxml2 */
			struct hwloc_xml_imported_distances_s {
				hwloc_obj_t root;
				struct hwloc_distances_s distances;
				struct hwloc_xml_imported_distances_s *prev, *next;
			} *first_distances, *last_distances;
		} xml;
		struct hwloc_backend_params_synthetic_s {
			/* synthetic backend parameters */
			char *string;
#define HWLOC_SYNTHETIC_MAX_DEPTH 128
			unsigned arity[HWLOC_SYNTHETIC_MAX_DEPTH];
			hwloc_obj_type_t type[HWLOC_SYNTHETIC_MAX_DEPTH];
			unsigned id[HWLOC_SYNTHETIC_MAX_DEPTH];
			unsigned depth[HWLOC_SYNTHETIC_MAX_DEPTH]; /* For cache/misc */
		} synthetic;
	} backend_params;
/* ********************************** */


	struct hwloc_backends_loaded* base_backends;
	struct hwloc_backends_loaded* global_backends;
	struct hwloc_backends_loaded* io_backends;
};

HWLOC_DECLSPEC void hwloc_setup_pu_level(struct hwloc_topology *topology, unsigned nb_pus);
extern int hwloc_get_sysctlbyname(const char *name, int64_t *n);
extern int hwloc_get_sysctl(int name[], unsigned namelen, int *n);
HWLOC_DECLSPEC unsigned hwloc_fallback_nbprocessors(struct hwloc_topology *topology);
extern void hwloc_connect_children(hwloc_obj_t obj);
extern int hwloc_connect_levels(hwloc_topology_t topology);


/*
 * Add an object to the topology.
 * It is sorted along the tree of other objects according to the inclusion of
 * cpusets, to eventually be added as a child of the smallest object including
 * this object.
 *
 * If the cpuset is empty, the type of the object (and maybe some attributes)
 * must be enough to find where to insert the object. This is especially true
 * for NUMA nodes with memory and no CPUs.
 *
 * The given object should not have children.
 *
 * This shall only be called before levels are built.
 *
 * In case of error, hwloc_report_os_error() is called.
 */
HWLOC_DECLSPEC void hwloc_insert_object_by_cpuset(struct hwloc_topology *topology, hwloc_obj_t obj);

/* Error reporting */
typedef void (*hwloc_report_error_t)(const char * msg, int line);
extern void hwloc_report_os_error(const char * msg, int line);
extern int hwloc_hide_errors(void);
/*
 * Add an object to the topology and specify which error callback to use
 */
extern int hwloc__insert_object_by_cpuset(struct hwloc_topology *topology, hwloc_obj_t obj, hwloc_report_error_t report_error);

/*
 * Insert an object somewhere in the topology.
 *
 * It is added as the last child of the given parent.
 * The cpuset is completely ignored, so strange objects such as I/O devices should
 * preferably be inserted with this.
 *
 * The given object may have children.
 *
 * Remember to call topology_connect() afterwards to fix handy pointers.
 */
extern void hwloc_insert_object_by_parent(struct hwloc_topology *topology, hwloc_obj_t parent, hwloc_obj_t obj);

/* Insert uname-specific names/values in the object infos array */
HWLOC_DECLSPEC void hwloc_add_uname_info(struct hwloc_topology *topology);


/* FIXME #ifdef HWLOC_INSIDE_LIBHWLOC */
/** \brief Return a locally-allocated stringified bitmap for printf-like calls. */
static __hwloc_inline char *
hwloc_bitmap_printf_value(hwloc_const_bitmap_t bitmap)
{
  char *buf;
  hwloc_bitmap_asprintf(&buf, bitmap);
  return buf;
}

static __hwloc_inline struct hwloc_obj *
hwloc_alloc_setup_object(hwloc_obj_type_t type, signed idx)
{
  struct hwloc_obj *obj = malloc(sizeof(*obj));
  memset(obj, 0, sizeof(*obj));
  obj->type = type;
  obj->os_index = idx;
  obj->os_level = -1;
  obj->attr = malloc(sizeof(*obj->attr));
  memset(obj->attr, 0, sizeof(*obj->attr));
  /* do not allocate the cpuset here, let the caller do it */
  return obj;
}

extern void hwloc_free_unlinked_object(hwloc_obj_t obj);

#define hwloc_object_cpuset_from_array(l, _value, _array, _max) do {	\
		struct hwloc_obj *__l = (l);				\
		unsigned int *__a = (_array);				\
		int k;							\
		__l->cpuset = hwloc_bitmap_alloc();			\
		for(k=0; k<_max; k++)					\
			if (__a[k] == _value)				\
				hwloc_bitmap_set(__l->cpuset, k);	\
	} while (0)

/* Configures an array of NUM objects of type TYPE with physical IDs OSPHYSIDS
 * and for which processors have ID PROC_PHYSIDS, and add them to the topology.
 * */
static __hwloc_inline void
hwloc_setup_level(int procid_max, unsigned num, unsigned *osphysids, unsigned *proc_physids, struct hwloc_topology *topology, hwloc_obj_type_t type)
{
  struct hwloc_obj *obj;
  unsigned j;

  hwloc_debug("%d %s\n", num, hwloc_obj_type_string(type));

  for (j = 0; j < num; j++)
    {
      obj = hwloc_alloc_setup_object(type, osphysids[j]);
      hwloc_object_cpuset_from_array(obj, j, proc_physids, procid_max);
      hwloc_debug_2args_bitmap("%s %d has cpuset %s\n",
		 hwloc_obj_type_string(type),
		 j, obj->cpuset);
      hwloc_insert_object_by_cpuset(topology, obj);
    }
  hwloc_debug("%s", "\n");
}
/* #endif */

/* This can be used for the alloc field to get allocated data that can be freed by free() */
void *hwloc_alloc_heap(hwloc_topology_t topology, size_t len);

/* This can be used for the alloc field to get allocated data that can be freed by munmap() */
void *hwloc_alloc_mmap(hwloc_topology_t topology, size_t len);

/* This can be used for the free_membind field to free data using free() */
int hwloc_free_heap(hwloc_topology_t topology, void *addr, size_t len);

/* This can be used for the free_membind field to free data using munmap() */
int hwloc_free_mmap(hwloc_topology_t topology, void *addr, size_t len);

/* Allocates unbound memory or fail, depending on whether STRICT is requested
 * or not */
static __hwloc_inline void *
hwloc_alloc_or_fail(hwloc_topology_t topology, size_t len, int flags)
{
  if (flags & HWLOC_MEMBIND_STRICT)
    return NULL;
  return hwloc_alloc(topology, len);
}

HWLOC_DECLSPEC void alloc_cpusets(hwloc_obj_t obj);

#endif /* HWLOC_TOPOLOGY_H */