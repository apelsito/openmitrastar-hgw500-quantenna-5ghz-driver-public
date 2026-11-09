#include "/workspace/user-home/Desktop/MitraStar/outputs/rea/linux73-full44-candidate-20261023T010500Z-r20/source/platform/ruby/dmautil.c"

static aligned_dma_descs race_owner;
static void publish_second_owner(void){assert(dmautil_aligned_dma_desc_alloc(&race_owner,16,4,32,false)==0);}
static void balanced(void){assert(record_live==0&&raw_live==0&&mapping_live==0);}
int main(void){
 unsigned cases=0;aligned_dma_descs d={0},unknown={0};
 assert(dmautil_aligned_dma_desc_alloc(NULL,16,4,32,false)==-EINVAL);cases++;
 assert(dmautil_aligned_dma_desc_alloc(&d,0,4,32,false)==-EINVAL);cases++;
 assert(dmautil_aligned_dma_desc_alloc(&d,16,0,32,false)==-EINVAL);cases++;
 for(unsigned a=0;a<31;a++){if(a&&(a&(a-1))==0)continue;assert(dmautil_aligned_dma_desc_alloc(&d,16,4,a,false)==-EINVAL);cases++;}
 for(unsigned backend=0;backend<2;backend++){
  for(unsigned shift=0;shift<8;shift++){
   unsigned a=1u<<shift;
   assert(dmautil_aligned_dma_desc_alloc(&d,16,4,a,backend)==0);
   assert(d.desc_count==4&&d.descs&&!(d.aligned_vdescs&(a-1)));
   aligned_dma_descs before=d;
   assert(dmautil_aligned_dma_desc_alloc(&d,16,4,a,backend)==-EBUSY);
   assert(!memcmp(&before,&d,sizeof(d)));
   /* Free uses the private record despite all public pointer fields corrupted. */
   memset(&d,0xa5,sizeof(d));
   dmautil_aligned_dma_desc_free(&d);balanced();
   assert(d.descs==NULL&&d.unaligned_vdescs==0);
   int frees=dram_free+sram_free;
   dmautil_aligned_dma_desc_free(&d);assert(frees==dram_free+sram_free);balanced();cases+=4;
  }
  for(unsigned stage=0;stage<3;stage++){
   if(stage==0)fail_record=1;if(stage==1)fail_raw=1;if(stage==2)fail_map=1;
   d=(aligned_dma_descs){0};
   assert(dmautil_aligned_dma_desc_alloc(&d,16,4,32,backend)==-ENOMEM);
   assert(!d.descs&&!d.unaligned_vdescs);balanced();cases++;
  }
 }
 unknown.unaligned_vdescs=1;unknown.descs=(void *)2;
 dmautil_aligned_dma_desc_free(&unknown);balanced();assert(unknown.unaligned_vdescs==1);cases++;
 dmautil_aligned_dma_desc_free(NULL);balanced();cases++;
 fixture_hook=publish_second_owner;
 assert(dmautil_aligned_dma_desc_alloc(&race_owner,16,4,32,true)==-EBUSY);
 assert(record_live==1&&raw_live==1&&mapping_live==1);
 dmautil_aligned_dma_desc_free(&race_owner);balanced();cases++;
 printf("PASS actual source ownership cases=%u dram_free=%d sram_free=%d unmaps=%d\n",cases,dram_free,sram_free,unmaps);
 return 0;
}
