#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#define __iomem
#define __init
#define __exit
#define GPIO_MAX 32
#define RUBY_GPIO_REGS_ADDR 0xf2000000U
static int owner;
#define THIS_MODULE (&owner)
struct gpio_chip{const char*label;void*owner;int(*direction_input)(struct gpio_chip*,unsigned);int(*direction_output)(struct gpio_chip*,unsigned,int);int(*set)(struct gpio_chip*,unsigned,int);int(*get)(struct gpio_chip*,unsigned);int base;unsigned ngpio;};struct arc_gpio_controller{unsigned unused;};struct arc_7XX_gpio{struct gpio_chip chip;struct arc_gpio_controller*regs;};static struct arc_7XX_gpio arc_gpio;
static int arc_gpio_dir_in(struct gpio_chip*c,unsigned i){(void)c;(void)i;return 0;}static int arc_gpio_dir_out(struct gpio_chip*c,unsigned i,int v){(void)c;(void)i;(void)v;return 0;}static int arc_gpio_set(struct gpio_chip*c,unsigned i,int v){(void)c;(void)i;(void)v;return 0;}static int arc_gpio_get(struct gpio_chip*c,unsigned i){(void)c;(void)i;return 0;}
static int addrc,added,removed;static struct gpio_chip*registered;
static int gpiochip_add_data(struct gpio_chip*c,void*d){assert(c->owner==THIS_MODULE&&d==&arc_gpio&&c->ngpio==32&&c->base==0);assert(c->set==arc_gpio_set&&c->get==arc_gpio_get);added++;if(addrc)return addrc;registered=c;return 0;}
static void gpiochip_remove(struct gpio_chip*c){assert(registered==c);registered=NULL;removed++;}
static int __init arc_gpio_init(void)
{
	memset(&arc_gpio, 0, sizeof(arc_gpio));
	arc_gpio.chip.label = "arcgpio";
	arc_gpio.chip.owner = THIS_MODULE;
	arc_gpio.chip.direction_input = arc_gpio_dir_in;
	arc_gpio.chip.direction_output = arc_gpio_dir_out;
	arc_gpio.chip.set = arc_gpio_set;
	arc_gpio.chip.get = arc_gpio_get;
	arc_gpio.chip.base = 0;
	arc_gpio.chip.ngpio = GPIO_MAX;

	arc_gpio.regs = (struct arc_gpio_controller *) RUBY_GPIO_REGS_ADDR;

	/* Preserve registration failure instead of reporting an unusable chip. */
	return gpiochip_add_data(&arc_gpio.chip, &arc_gpio);
}static void __exit arc_gpio_exit(void)
{
	gpiochip_remove(&arc_gpio.chip);
}int main(void){int rc[]={-12,-22,-16,0};unsigned cases=0;for(unsigned i=0;i<4;i++){addrc=rc[i];added=removed=0;registered=NULL;assert(arc_gpio_init()==rc[i]&&added==1&&arc_gpio.chip.owner==THIS_MODULE);if(!rc[i]){arc_gpio_exit();assert(!registered&&removed==1);}else assert(!registered&&!removed);cases++;}printf("{\"actual_GPIO_init_exit_cases\":%u}\n",cases);return 0;}
