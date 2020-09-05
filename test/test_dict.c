#include "src/dict.h"

#include <assert.h>



int
main()
{
    Dict *d = dict_init();
    Dict *d3 = dict_init();

    d3 = dict_set(d3, "c", "china888");
    d3 = dict_set(d3, "haha", "ffffff");
    
    d = dict_set(d, "c", "china");
    d = dict_set(d, "gxg", "45");
    d = dict_set(d, "gxg", "18");
    d = dict_set(d, "hhh", "修改过的");

    dict_update(d, d3);

    dict_dispaly(d);
    printf("c: %s\n", dict_get(d, "c"));
    printf("gxg: %s\n", dict_get(d, "gxg"));
    printf("haha: %s\n", dict_get(d, "haha"));
    
    char **rv = (char **) malloc(sizeof(char **) * 132);
    rv = dict_keys(rv, d);
    printf("keys: %s %s %s\n", rv[0], rv[1], rv[2]);
    dict_free(d);
    // dict_dispaly(d);
}
