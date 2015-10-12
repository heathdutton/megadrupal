Field index [![Build Status](https://travis-ci.org/atdrupal/field_index.module.svg)](https://travis-ci.org/atdrupal/field_index.module)
====

Field modules don't always add indexes for its columns (value column of text
field for example), this leads to poor performance in views when we would like
sort or filter views using the un-indexed columns. We can check the query
executed by views and add custom indexes manually, but it's not easy to deploy.

With field_index module, we can create custom indexes for fields. The indexes are
exportable.

Module is sponsored by [Red Airship](http://www.redairship.com/).
