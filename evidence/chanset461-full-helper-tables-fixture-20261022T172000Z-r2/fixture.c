#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "openmitrastar_chanset_recovered.h"
static const uint32_t expected[130][20] = {
{1U,0U,20U,1U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{2U,0U,20U,2U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{3U,0U,20U,3U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{4U,0U,20U,4U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{5U,0U,20U,5U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{6U,0U,20U,6U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{7U,0U,20U,7U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{8U,0U,20U,8U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{9U,0U,20U,9U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{10U,0U,20U,10U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{11U,0U,20U,11U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{12U,0U,20U,12U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{13U,0U,20U,13U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{14U,0U,20U,14U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{1U,1U,40U,3U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{2U,1U,40U,4U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{3U,1U,40U,5U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{4U,1U,40U,6U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{5U,1U,40U,7U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{6U,1U,40U,8U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{7U,1U,40U,9U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{8U,1U,40U,10U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{9U,1U,40U,11U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{5U,3U,40U,3U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{6U,3U,40U,4U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{7U,3U,40U,5U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{8U,3U,40U,6U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{9U,3U,40U,7U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{10U,3U,40U,8U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{11U,3U,40U,9U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{12U,3U,40U,10U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{13U,3U,40U,11U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{36U,0U,20U,36U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{40U,0U,20U,40U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{44U,0U,20U,44U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{48U,0U,20U,48U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{52U,0U,20U,52U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{56U,0U,20U,56U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{60U,0U,20U,60U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{64U,0U,20U,64U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{100U,0U,20U,100U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{104U,0U,20U,104U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{108U,0U,20U,108U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{112U,0U,20U,112U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{116U,0U,20U,116U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{120U,0U,20U,120U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{124U,0U,20U,124U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{128U,0U,20U,128U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{132U,0U,20U,132U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{136U,0U,20U,136U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{140U,0U,20U,140U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{144U,0U,20U,144U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{149U,0U,20U,149U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{153U,0U,20U,153U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{157U,0U,20U,157U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{161U,0U,20U,161U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{165U,0U,20U,165U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{169U,0U,20U,169U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{184U,0U,20U,184U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{188U,0U,20U,188U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{192U,0U,20U,192U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{196U,0U,20U,196U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{36U,1U,40U,38U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{40U,3U,40U,38U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{44U,1U,40U,46U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{48U,3U,40U,46U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{52U,1U,40U,54U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{56U,3U,40U,54U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{60U,1U,40U,62U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{64U,3U,40U,62U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{100U,1U,40U,102U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{104U,3U,40U,102U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{108U,1U,40U,110U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{112U,3U,40U,110U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{116U,1U,40U,118U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{120U,3U,40U,118U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{124U,1U,40U,126U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{128U,3U,40U,126U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{132U,1U,40U,134U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{136U,3U,40U,134U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{140U,1U,40U,142U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{144U,3U,40U,142U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{149U,1U,40U,151U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{153U,3U,40U,151U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{157U,1U,40U,159U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{161U,3U,40U,159U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{184U,1U,40U,186U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{188U,3U,40U,186U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{192U,1U,20U,194U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{196U,3U,20U,194U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{36U,1U,80U,42U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{40U,3U,80U,42U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{44U,1U,80U,42U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{48U,3U,80U,42U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{52U,1U,80U,58U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{56U,3U,80U,58U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{60U,1U,80U,58U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{64U,3U,80U,58U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{100U,1U,80U,106U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{104U,3U,80U,106U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{108U,1U,80U,106U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{112U,3U,80U,106U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{116U,1U,80U,122U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{120U,3U,80U,122U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{124U,1U,80U,122U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{128U,3U,80U,122U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{132U,1U,80U,138U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{136U,3U,80U,138U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{140U,1U,80U,138U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{144U,3U,80U,138U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{149U,1U,80U,155U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{153U,3U,80U,155U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{157U,1U,80U,155U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{161U,3U,80U,155U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{36U,1U,160U,50U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{40U,3U,160U,50U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{44U,1U,160U,50U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{48U,3U,160U,50U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{52U,1U,160U,50U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{56U,3U,160U,50U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{60U,1U,160U,50U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{64U,3U,160U,50U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{100U,1U,160U,114U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{104U,3U,160U,114U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{108U,1U,160U,114U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{112U,3U,160U,114U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{116U,1U,160U,114U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{120U,3U,160U,114U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{124U,1U,160U,114U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
{128U,3U,160U,114U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U},
};

int main(void) {
    struct om_chanset_view view, sentinel;
    const struct om_chanset_entry *found, *expected_found;
    const unsigned bands[6] = {0,0,1,1,1,1};
    const unsigned widths[6] = {20,40,20,40,80,160};
    const unsigned counts[6] = {14,18,30,28,24,16};
    const unsigned queries[6] = {0,20,40,80,160,161};
    unsigned table, index, word, channel, width, secondary, all=0, exact=0, base=0;
    for(table=0;table<6;table++) {
        assert(om_chanset_template(bands[table],widths[table],&view)==0);
        assert(view.count==counts[table] && view.capacity==counts[table]);
        for(index=0;index<view.count;index++) {
            const uint32_t *compiled=(const uint32_t*)&view.entries[index];
            for(word=0;word<20;word++) {assert(compiled[word]==expected[base+index][word]);exact++;}
        }
        for(channel=0;channel<256;channel++)for(width=0;width<6;width++)for(secondary=0;secondary<5;secondary++) {
            int result;
            expected_found=0;
            for(index=0;index<view.count;index++) {
                const uint32_t *row=expected[base+index];
                if(row[0]==channel && row[2]==queries[width] && row[1]==secondary) {
                    expected_found=&view.entries[index];break;
                }
            }
            found=(const struct om_chanset_entry*)(uintptr_t)1;
            result=om_chanset_find(&view,channel,queries[width],secondary,&found);
            assert(result==!!expected_found && found==expected_found);all++;
        }
        base+=view.count;
    }
    sentinel.entries=(const struct om_chanset_entry*)(uintptr_t)1; sentinel.count=7;sentinel.capacity=8;
    view=sentinel;
    assert(om_chanset_template(9,20,&view)==-22 && view.entries==sentinel.entries && view.count==7);
    assert(om_chanset_template(0,80,&view)==-22 && view.entries==sentinel.entries && view.count==7);
    assert(om_chanset_template(1,21,&view)==-22 && view.entries==sentinel.entries && view.count==7);
    assert(om_chanset_template(0,20,0)==-22);
    found=(const struct om_chanset_entry*)(uintptr_t)3;
    assert(om_chanset_find(0,1,20,0,&found)==-22 && (uintptr_t)found==3);
    view=sentinel;view.count=9;
    assert(om_chanset_find(&view,1,20,0,&found)==-22 && (uintptr_t)found==3);
    view=sentinel;view.capacity=99;
    assert(om_chanset_find(&view,1,20,0,&found)==-22 && (uintptr_t)found==3);
    view.entries=0;view.count=0;view.capacity=0;
    assert(om_chanset_find(&view,1,20,0,&found)==-22 && (uintptr_t)found==3);
    assert(om_chanset_find(&sentinel,1,20,0,0)==-22);
    { /* duplicate keys must select the first original record */
        struct om_chanset_entry duplicate[2]={{0}};
        duplicate[0].primary=duplicate[1].primary=1;
        duplicate[0].bandwidth=duplicate[1].bandwidth=20;
        view.entries=duplicate;view.count=2;view.capacity=2;
        assert(om_chanset_find(&view,1,20,0,&found)==1 && found==duplicate);
    }
    printf("{\"lookup_cases\":%u,\"exact_compiled_words\":%u,\"negative_cases\":9,\"duplicate_first_match\":true}\n",all,exact);
    return 0;
}

