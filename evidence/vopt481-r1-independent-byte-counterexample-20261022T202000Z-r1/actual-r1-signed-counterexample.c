
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "openmitrastar_vopt481_recovered.h"
static int values[500];static unsigned seen[500];
static int param(void*x,unsigned id,int v){(void)x;assert(id<500);values[id]=v;seen[id]++;return 0;}
static int scs(void*x,unsigned v){(void)x;(void)v;return 1;}
static int ocac(void*x,unsigned v){(void)x;(void)v;return 0;}
static void reassoc(void*x,unsigned a,unsigned b){(void)x;(void)a;(void)b;}
int main(void){struct om_vopt_state s;struct om_vopt_environment e={0,0,0,255};struct om_vopt_provider p={0,param,scs,ocac,0,0,reassoc};om_vopt_initialize(&s,255,255);s.saved_txbf=255;s.saved_probe=128;assert(!om_vopt_apply(&s,0,&e,&p));assert(values[78]==-1&&values[158]==-1&&values[195]==-128&&values[288]==-1&&values[482]==-1);printf("{\"cases\":1,\"saved78_actual\":%d,\"saved78_oem_expected\":255,\"saved195_actual\":%d,\"saved195_oem_expected\":128,\"mode482_actual\":%d,\"mode482_oem_expected\":-1}\n",values[78],values[195],values[482]);return 0;}
