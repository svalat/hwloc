/* 
 * Copyright © 2009 CNRS, INRIA, Université Bordeaux 1
 *
 * This software is a computer program whose purpose is to provide
 * abstracted information about the hardware topology.
 *
 * This software is governed by the CeCILL-B license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL-B
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

#include <topology.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* check that object userdata is properly initialized */

static void check(topo_topology_t topology)
{
  struct topo_topology_info topoinfo;
  unsigned i,j;

  topo_topology_get_info(topology, &topoinfo);
  for(i=0; i<topoinfo.depth; i++) {
    for(j=0; j<topo_get_depth_nbobjects(topology, i); j++) {
      assert(topo_get_object(topology, i, j)->userdata == NULL);
    }
  }
}

int main(void)
{
  topo_topology_t topology;

  /* check the real topology */
  topo_topology_init(&topology);
  topo_topology_load(topology);
  check(topology);
  topo_topology_destroy(topology);

  /* check a synthetic topology */
  topo_topology_init(&topology);
  topo_topology_set_synthetic(topology, "6 5 4 3 2");
  topo_topology_load(topology);
  check(topology);
  topo_topology_destroy(topology);

  return 0;
}
