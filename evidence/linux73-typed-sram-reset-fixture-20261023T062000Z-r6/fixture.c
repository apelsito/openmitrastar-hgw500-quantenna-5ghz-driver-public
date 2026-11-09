#include "/workspace/user-home/Desktop/MitraStar/outputs/rea/linux73-typed-sram-reset-20261023T061000Z-r6/om_sram_core.c"
#include "/workspace/user-home/Desktop/MitraStar/outputs/rea/linux73-typed-sram-reset-20261023T061000Z-r6/om73_sram_backend.c"

static struct om73_sram_board_contract valid(void){
 struct om73_sram_board_contract b={0};
 b.arena=(struct om_sram_contract){.enabled=1,.reserved_exclusive=1,.complete_occupancy_map=1,
 .cpu_base=0x88000000,.bus_base=0x80000000,.uncached_base=0x60000000,.total_bytes=0x80000,
 .start=0x12000,.end=0x14000,.kernel_occupied_end=0x12000};
 b.kernel_occupancy_sha256[0]=1;b.selected_carriers_sha256[0]=2;b.complete_writers_map_sha256[0]=3;return b;
}
static void closed(void){assert(!resource_live&&!mapping_live&&!module_live&&!spin_depth&&!mutex_depth);}
int fixture_core_main(void){
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

static atomic_int started,completed;
static int threaded_result,thread_mode;
static struct om73_sram_writer_token *foreign_token;
static void *operation(void *arg){
 (void)arg;atomic_store(&started,1);
 if(thread_mode==0){struct om73_sram_writer_token token;threaded_result=om73_sram_writer_begin(1,&token);if(!threaded_result)threaded_result=om73_sram_writer_end(&token);}
 else if(thread_mode==1){struct om73_sram_board_contract b=valid();threaded_result=om73_sram_acquire(&b);}
 else if(thread_mode==2)threaded_result=om73_sram_close();
 else if(thread_mode==3)threaded_result=om73_sram_writer_end(foreign_token);
 atomic_store(&completed,1);return NULL;
}
static pthread_t start_thread(int mode){
 pthread_t thread;thread_mode=mode;atomic_store(&started,0);atomic_store(&completed,0);
 assert(!pthread_create(&thread,NULL,operation,NULL));
 for(unsigned n=0;!atomic_load(&started)&&n<1000000;n++)sched_yield();
 assert(atomic_load(&started));return thread;
}
static void blocked(void){usleep(20000);assert(!atomic_load(&completed));}
int fixture_writer_main(void){
 int baseline=fixture_core_main();assert(!baseline);unsigned cases=362;
 struct om73_sram_writer_token token={0},copy,old;
 struct om73_sram_board_contract b=valid();struct om73_sram_allocation allocation;int owner;
 assert(om73_sram_writer_begin(3,&token)==-EINVAL);cases++;
 assert(om73_sram_writer_begin(0,NULL)==-EINVAL);cases++;
 assert(om73_sram_writer_check(&token,0x88012000,1)==-EPERM);cases++;
 assert(om73_sram_writer_end(&token)==-EPERM);cases++;
 fail_module=1;assert(om73_sram_writer_begin(0,&token)==-ENODEV);fail_module=0;closed();cases++;
 om73_writer_generation=U64_MAX;assert(om73_sram_writer_begin(0,&token)==-EOVERFLOW);om73_writer_generation=0;closed();cases++;
 assert(om73_sram_writer_begin(0,&token)==0);assert(module_live==1&&mutex_depth==1);cases++;
 assert(om73_sram_writer_check(&token,0x88012000,8192)==0);cases++;
 assert(om73_sram_writer_check(&token,0xffffffff,2)==-ERANGE);cases++;
 assert(om73_sram_writer_check(&token,0,0)==-ERANGE);cases++;
 copy=token;assert(om73_sram_writer_end(&copy)==-EPERM);assert(mutex_depth==1&&module_live==1);cases++;
 old=token;token.generation--;assert(om73_sram_writer_end(&token)==-EPERM);token=old;cases++;
 token.cpu=2;assert(om73_sram_writer_check(&token,0x88000000,1)==-EPERM);token=old;cases++;
 assert(om73_sram_writer_begin(1,&copy)==-EBUSY);cases++;
 assert(om73_sram_acquire(&b)==-EBUSY);cases++;
 assert(om73_sram_close()==-EBUSY);cases++;
 foreign_token=&token;pthread_t foreign=start_thread(3);assert(!pthread_join(foreign,NULL));assert(threaded_result==-EPERM);assert(module_live==1&&mutex_depth==1);cases++;
 pthread_t waiter=start_thread(0);blocked();assert(om73_sram_writer_end(&token)==0);assert(!pthread_join(waiter,NULL));assert(threaded_result==0);closed();cases++;
 assert(om73_sram_writer_end(&token)==-EPERM);cases++;
 assert(om73_sram_writer_begin(2,&token)==0);
 waiter=start_thread(1);blocked();assert(om73_sram_writer_end(&token)==0);assert(!pthread_join(waiter,NULL));assert(threaded_result==0);assert(resource_live==1&&mapping_live==1&&module_live==1);cases++;
 for(unsigned cpu=0;cpu<3;cpu++){
  assert(om73_sram_writer_begin(cpu,&token)==0);assert(module_live==2);
  /* Free-list metadata protects the full arena even with live_count==0. */
  assert(om73_core.live_count==0);
  assert(om73_sram_writer_check(&token,0x88012000,8192)==-EBUSY);
  assert(om73_sram_writer_check(&token,0x88011fff,2)==-EBUSY);
  assert(om73_sram_writer_check(&token,0x88013fff,2)==-EBUSY);
  assert(om73_sram_writer_check(&token,0x88014000,1)==0);
  assert(om73_sram_writer_check(&token,0x88011fff,1)==0);
  assert(om73_sram_alloc(&owner,64,64,&allocation)==0);
  assert(om73_sram_writer_check(&token,allocation.cpu_phys,1)==-EBUSY);
  assert(om73_sram_free(&owner)==0);
  assert(om73_sram_writer_end(&token)==0);assert(module_live==1);cases+=9;
 }
 assert(om73_sram_writer_begin(0,&token)==0);
 waiter=start_thread(2);blocked();assert(om73_sram_writer_end(&token)==0);assert(!pthread_join(waiter,NULL));assert(threaded_result==0);closed();cases++;
 printf("PASS actual task writer lease cases=%u concurrent_waiters=3 foreign_end=refused resource=%u/%u map=%u/%u\n",cases,request_calls,release_calls,map_calls,unmap_calls);
 return 0;
}

static struct om73_sram_reset_token *foreign_reset;
static int reset_thread_mode;
static void *reset_operation(void *arg){
 (void)arg;atomic_store(&started,1);
 if(reset_thread_mode==0){struct om73_sram_reset_token token;threaded_result=om73_sram_reset_begin(0x4180,&token);if(!threaded_result)threaded_result=om73_sram_reset_end(&token);}
 else if(reset_thread_mode==1)threaded_result=om73_sram_reset_end(foreign_reset);
 else if(reset_thread_mode==2){struct om73_sram_writer_token token;threaded_result=om73_sram_writer_begin(1,&token);if(!threaded_result)threaded_result=om73_sram_writer_end(&token);}
 else if(reset_thread_mode==3){struct om73_sram_board_contract b=valid();threaded_result=om73_sram_acquire(&b);}
 else if(reset_thread_mode==4)threaded_result=om73_sram_close();
 else if(reset_thread_mode==5){int owner;struct om73_sram_allocation allocation;threaded_result=om73_sram_alloc(&owner,64,64,&allocation);}
 atomic_store(&completed,1);return NULL;
}
static pthread_t start_reset_thread(int mode){
 pthread_t thread;reset_thread_mode=mode;atomic_store(&started,0);atomic_store(&completed,0);
 assert(!pthread_create(&thread,NULL,reset_operation,NULL));
 for(unsigned n=0;!atomic_load(&started)&&n<1000000;n++)sched_yield();
 assert(atomic_load(&started));return thread;
}
int main(void){
 assert(fixture_writer_main()==0);unsigned cases=410;
 struct om73_sram_board_contract b=valid();struct om73_sram_reset_token token={0},copy,old;
 struct om73_sram_writer_token writer;struct om73_sram_allocation allocation;int owner;
 for(unsigned bit=0;bit<32;bit++)if((1u<<bit)!=0x4180){assert(om73_sram_reset_begin(1u<<bit,&token)==-EINVAL);cases++;}
 assert(om73_sram_reset_begin(0,&token)==-EINVAL);cases++;
 assert(om73_sram_reset_begin(0x4180,NULL)==-EINVAL);cases++;
 assert(om73_sram_reset_end(&token)==-EPERM);cases++;
 fail_module=1;assert(om73_sram_reset_begin(0x4180,&token)==-ENODEV);fail_module=0;closed();cases++;
 om73_reset_generation=U64_MAX;assert(om73_sram_reset_begin(0x4180,&token)==-EOVERFLOW);om73_reset_generation=0;closed();cases++;
 assert(om73_sram_writer_begin(0,&writer)==0);
 assert(om73_sram_reset_begin(0x4180,&token)==-EBUSY);cases++;
 pthread_t waiter=start_reset_thread(0);blocked();assert(om73_sram_writer_end(&writer)==0);assert(!pthread_join(waiter,NULL));assert(threaded_result==0);closed();cases++;
 assert(om73_sram_acquire(&b)==0);assert(om73_sram_alloc(&owner,64,64,&allocation)==0);
 unsigned refs=module_live;assert(om73_sram_reset_begin(0x4180,&token)==-EBUSY);assert(module_live==(int)refs);cases++;
 assert(om73_sram_query(&owner,&allocation)==0);assert(om73_sram_free(&owner)==0);
 /* Detect inconsistent retained ledger even if the core count was cleared. */
 om73_owned[0].owner=&owner;assert(om73_sram_reset_begin(0x4180,&token)==-EBUSY);om73_owned[0].owner=NULL;cases++;
 assert(om73_sram_reset_begin(0x4180,&token)==0);assert(module_live==2&&mutex_depth==1);cases++;
 copy=token;assert(om73_sram_reset_end(&copy)==-EPERM);cases++;
 old=token;token.generation--;assert(om73_sram_reset_end(&token)==-EPERM);token=old;cases++;
 token.mask++;assert(om73_sram_reset_end(&token)==-EPERM);token=old;cases++;
 assert(om73_sram_reset_begin(0x4180,&copy)==-EBUSY);cases++;
 assert(om73_sram_writer_begin(0,&writer)==-EBUSY);cases++;
 assert(om73_sram_acquire(&b)==-EBUSY);cases++;
 assert(om73_sram_close()==-EBUSY);cases++;
 assert(om73_sram_alloc(&owner,64,64,&allocation)==-EBUSY);cases++;
 assert(om73_sram_query(&owner,&allocation)==-EBUSY);cases++;
 assert(om73_sram_free(&owner)==-EBUSY);cases++;
 foreign_reset=&token;pthread_t foreign=start_reset_thread(1);assert(!pthread_join(foreign,NULL));assert(threaded_result==-EPERM);assert(module_live==2&&mutex_depth==1);cases++;
 foreign=start_reset_thread(5);assert(!pthread_join(foreign,NULL));assert(threaded_result==-EBUSY);cases++;
 waiter=start_reset_thread(2);blocked();assert(om73_sram_reset_end(&token)==0);assert(!pthread_join(waiter,NULL));assert(threaded_result==0&&module_live==1);cases++;
 assert(om73_sram_alloc(&owner,64,64,&allocation)==0);assert(om73_sram_free(&owner)==0);cases++;
 /* Reset admission does not touch preserved free-list state. */
 unsigned long free_bytes=om73_core.memlist.length;assert(om73_sram_reset_begin(0x4180,&token)==0);
 waiter=start_reset_thread(4);blocked();assert(om73_core.memlist.length==free_bytes);assert(om73_sram_reset_end(&token)==0);assert(!pthread_join(waiter,NULL));assert(threaded_result==0);closed();cases++;
 assert(om73_sram_reset_begin(0x4180,&token)==0);
 waiter=start_reset_thread(3);blocked();assert(om73_sram_reset_end(&token)==0);assert(!pthread_join(waiter,NULL));assert(threaded_result==0);assert(om73_sram_close()==0);closed();cases++;
 printf("PASS actual reset software lease cases=%u mask=0x4180 hardware_reset_writes=0 concurrent_waiters=4\n",cases);
 return 0;
}
