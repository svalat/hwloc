/*
 * Copyright © 2009 CNRS
 * Copyright © 2009-2012 Inria.  All rights reserved.
 * Copyright © 2009-2010 Université Bordeaux 1
 * Copyright © 2009-2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

#include <private/autogen/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lstopo.h"
#include "hwloc.h"

struct native_svg_context
{
	FILE * fp;
};

/* Cairo methods */
static void topo_native_svg_box(void *output, int r, int g, int b, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned width, unsigned y, unsigned height,hwloc_obj_t level)
{
	struct native_svg_context * cs = output;
	char type[128];
	hwloc_obj_type_snprintf(type, sizeof(type), level, 0);
	if (type != NULL && level->logical_index >= 0)
		fprintf(cs->fp,"\t<rect id='rect_%s_%d' class='%s' x='%d' y='%d' width='%d' height='%d' style='fill:rgb(%d,%d,%d);stoke-width:1;stroke:rgb(0,0,0)'/>\n",type,level->logical_index,type,x,y,width,height,r,g,b);
	else
		fprintf(cs->fp,"\t<rect x='%d' y='%d' width='%d' height='%d' style='fill:rgb(%d,%d,%d);stoke-width:1;stroke:rgb(0,0,0)'/>\n",x,y,width,height,r,g,b);
}

static void
topo_native_svg_line(void *output, int r, int g, int b, unsigned depth __hwloc_attribute_unused, unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
	struct native_svg_context * cs = output;
	fprintf(cs->fp,"\t<line x1='%d' y1='%d' x2='%d' y2='%d' style='stroke:rgb(%d,%d,%d);stroke-width:1;'/>\n",x1,y1,x2,y2,r,g,b);
}

static void
topo_native_svg_text(void *output, int r, int g, int b, int size, unsigned depth __hwloc_attribute_unused, unsigned x, unsigned y, const char *text,hwloc_obj_t level)
{
	struct native_svg_context * cs = output;
	char type[128];
	hwloc_obj_type_snprintf(type, sizeof(type), level, 0);
	if (type != NULL && level->logical_index >= 0)
		fprintf(cs->fp,"\t<text id='text_%s_%d' class='%s' x='%d' y='%d' fill='rgb(%d,%d,%d)' style='font-size:%dpx'>%s</text>\n",type,level->logical_index,type,x,y+size,r,g,b,size,text);
	else
		fprintf(cs->fp,"\t<text x='%d' y='%d' fill='rgb(%d,%d,%d)' style='font-size:%d'>%s</text>\n",x,y+size,r,g,b,size,text);
}

static void
topo_native_svg_paint(struct draw_methods *methods, int logical, int legend, hwloc_topology_t topology,struct native_svg_context * cs)
{
	output_draw(methods, logical, legend, topology, cs);
}

static void null_declare_color (void *output __hwloc_attribute_unused, int r __hwloc_attribute_unused, int g __hwloc_attribute_unused, int b __hwloc_attribute_unused) {}

/* SVG back-end */
static void * native_svg_start(void *output, int width, int height)
{
	struct native_svg_context * cs = output;

	fprintf(cs->fp,"%s","<?xml version='1.0' encoding='UTF-8'?>\n");
	fprintf(cs->fp,"<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='%dpx' height='%dpx' viewBox='0 0 %dpx %dpx' version='1.1'>\n",width,height,width,height);
	return output;
}

static struct draw_methods native_svg_draw_methods = {
	native_svg_start,
	null_declare_color,
	topo_native_svg_box,
	topo_native_svg_line,
	topo_native_svg_text,
};

void print_svg_foot(struct native_svg_context cs)
{
	fprintf(cs.fp,"</svg>\n");
}

void output_native_svg(hwloc_topology_t topology, const char *filename, int logical, int legend, int verbose_mode __hwloc_attribute_unused)
{
	//vars
	struct native_svg_context cs;

	//open file
	if (filename == NULL)
		cs.fp = stdout;
	else
		cs.fp = fopen(filename,"w");
	if (!cs.fp) {
		perror(filename);
		fprintf(stderr, "Failed to open %s for writing (%s)\n", filename, strerror(errno));
		return;
	}

	//down
	output_draw_start(&native_svg_draw_methods, logical, legend, topology, &cs);
	topo_native_svg_paint(&native_svg_draw_methods, logical, legend, topology, &cs);
	print_svg_foot(cs);

	//close file
	if (cs.fp != stdout)
		fclose(cs.fp);
}
