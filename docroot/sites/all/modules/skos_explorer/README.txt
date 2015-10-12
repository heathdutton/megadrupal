For testing using Devel module

1. Print tree given a URI
$leafs = skosapi_get_parents_nodes('http://aims.fao.org/aos/agrovoc/c_330606');
dpm(skosapi_root_search($leafs));

2. Print children given a URI
dpm(skos_explorer_children('http://aims.fao.org/aos/agrovoc/c_330998'));

