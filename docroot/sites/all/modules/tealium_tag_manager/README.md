Tealium is an API which allows third party apps to add its own tags into the site.
The module itself does not do anything, you need to develop your own integration,
although a Tealium UI for some basic stuff is planned at some point.

##Adding new tags.

Tealium uses hook_tealium_tags to communicate the new tags we want to add to a
new page. Let’s say for example we are working in a module, called my_module.
In my_module.module we’ll have a function like that:

```
function my_module_tealium_tags() {
  return $tags;
}
```

Where tags will be an array with the tags that we want to populate.
For example:

```
  $tags['pageTemplate'] = 'My Template';
```

##TROUBLESHOOTING
- Wrong encoding for some variables:
In case you need to add a tag with special encodings, like 'test & test', the
& will be transformed in something like 'test &amp; test'.

To avoid this simply add the tag you need with special coding to, ie:

```
function mymodule_update_7001() {
  $non_filtered = array('pageName', 'pageTemplate');

  variable_set('tealium_no_filter_xss_list', $non_filtered);
}
```

So from now, you pageTemplate tag will accept special coded characters.
