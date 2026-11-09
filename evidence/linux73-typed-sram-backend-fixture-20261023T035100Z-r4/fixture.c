#include "/workspace/user-home/Desktop/MitraStar/outputs/rea/linux73-typed-sram-backend-20261023T034500Z-r4/om_sram_core.c"
#include "/workspace/user-home/Desktop/MitraStar/outputs/rea/linux73-typed-sram-backend-20261023T034500Z-r4/om73_sram_backend.c"

static struct om73_sram_board_contract valid(void){
 struct om73_sram_board_contract b={0};
 b.arena=(struct om_sram_contract){.enabled=1,.reserved_exclusive=1,.complete_occupancy_map=1,
 .cpu_base=0x88000000,.bus_base=0x80000000,.uncached_base=0x60000000,.total_bytes=0x80000,
 .start=0x12000,.end=0x14000,.kernel_occupied_end=0x12000};
 b.kernel_occupancy_sha256[0]=1;b.selected_carriers_sha256[0]=2;b.complete_writers_map_sha256[0]=3;return b;
}
static void closed(void){assert(!resource_live&&!mapping_live&&!module_live&&!spin_depth&&!mutex_depth);}
int main(void){
 unsigned cases=0;struct om73_sram_board_contract b=valid(),bad;struct om73_sram_allocation a,q;
 int owner[129];
 memset(storage,0xa5,sizeof(storage));
 assert(om73_sram_alloc(&owner[0],16,16,&a)==-ENODEV);cases++;
 assert(om73_sram_query(&owner[0],&a)==-ENODEV);cases++;
 assert(om73_sram_free(&owner[0])==-ENODEV);cases++;
 assert(om73_sram_close()==-ENODEV);closed();cases++;
 assert(om73_sram_acquire(NULL)==-EINVAL);closed();cases++;
 for(unsigned mode=0;mode<11;mode++){
  bad=b;
  switch(mode){case 0:bad.arena.enabled=0;break;case 1:bad.arena.reserved_exclusive=0;break;
   case 2:bad.arena.complete_occupancy_map=0;break;case 3:bad.arena.cpu_base++;break;
   case 4:bad.arena.start++;break;case 5:bad.arena.end++;break;case 6:bad.arena.kernel_occupied_end=0;break;
   case 7:bad.arena.occupied[0]=(struct om_sram_range){0x11000,0x13000};bad.arena.noccupied=1;break;
   case 8:memset(bad.kernel_occupancy_sha256,0,32);break;
   case 9:memset(bad.selected_carriers_sha256,0,32);break;
   case 10:memset(bad.complete_writers_map_sha256,0,32);break;}
  assert(om73_sram_acquire(&bad)==-EINVAL);closed();assert(storage[0]==0xa5);cases++;
 }
 fail_module=1;assert(om73_sram_acquire(&b)==-ENODEV);fail_module=0;closed();cases++;
 fail_resource=1;assert(om73_sram_acquire(&b)==-EBUSY);fail_resource=0;closed();cases++;
 fail_mapping=1;assert(om73_sram_acquire(&b)==-ENOMEM);fail_mapping=0;closed();cases++;
 assert(om73_sram_acquire(&b)==0);assert(resource_live&&mapping_live&&module_live);cases++;
 assert(om73_sram_acquire(&b)==-EBUSY);cases++;
 for(unsigned shift=0;shift<=13;shift++){
  unsigned align=1u<<shift;
  int result=om73_sram_alloc(&owner[0],16,align,&a);
  if(align==8192){assert(result==-ENOMEM);cases++;continue;}
  assert(result==0);assert(!(a.cpu_phys&(align-1)));assert(!(a.bus&(align-1)));
  assert(a.bus==a.cpu_phys-0x08000000ul);
  assert((unsigned char *)a.io_map==storage+(a.cpu_phys-0x88012000ul));
  assert(a.size==16&&a.generation);
  assert(om73_sram_query(&owner[0],&q)==0&&!memcmp(&a,&q,sizeof(a)));
  assert(om73_sram_close()==-EBUSY);assert(module_live&&mapping_live&&resource_live);
  assert(om73_sram_alloc(&owner[0],16,align,&q)==-EBUSY);
  assert(om73_sram_free(&owner[0])==0);assert(om73_sram_free(&owner[0])==-ENOENT);cases+=6;
 }
 assert(om73_sram_alloc(NULL,16,16,&a)==-EINVAL);cases++;
 assert(om73_sram_alloc(&owner[0],0,16,&a)==-EINVAL);cases++;
 assert(om73_sram_alloc(&owner[0],16,3,&a)==-EINVAL);cases++;
 assert(om73_sram_alloc(&owner[0],SIZE_MAX,8192,&a)==-EOVERFLOW);cases++;
 for(unsigned i=0;i<128;i++)assert(om73_sram_alloc(&owner[i],1,1,&a)==0);
 assert(om73_sram_alloc(&owner[128],1,1,&a)==-ENOMEM);cases+=129;
 for(unsigned i=0;i<128;i++)assert(om73_sram_free(&owner[i])==0);cases+=128;
 assert(om73_sram_close()==0);closed();cases++;
 assert(release_calls==unmap_calls+1); /* mappingfail released resource only */
 printf("PASS actual backend resource+mapping+ownership cases=%u request=%u release=%u map=%u unmap=%u\n",cases,request_calls,release_calls,map_calls,unmap_calls);
 return 0;
}
