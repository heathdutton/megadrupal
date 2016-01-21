Description
-----------

Defacto lets you mark specific nodes as the default or canonical response to
specific search keywords. This means it will be pushed to the top of search
results regardless of keywords that appear in the body, title, or other fields.

To do this, it adjusts the bias settings on a specific taxonomy term reference
field applied to a node (or potentially other content searchable with the
Apache Solr Search module).

Why not just bias a taxonomy item? Because we want to be able to set a specific
document as the default search result, vs. all nodes tagged with a specific
taxonomy, so that's why we tie the boosting to a specific taxonomy reference
field, vs just boosting the taxonomy itself.

So the idea is, if you set the taxonomy referernce field to a given keyword,
that node will be boosted but just tagging a node with that taxonomy in a
different reference wouldn't.

Usage
-----

- Enable the Defacto module at Administration > Modules.
- Create or choose a Vocabulary to use as the Defacto selector.
- Add this Vocabulary as a Term Reference field to one or more content types 
- In the Defacto configration at Administration > Configuration > Defacto
  (under the Search group), adjust the Search Bias (if desired) and select the
  taxonomy reference field you wish to use with Defacto.
  
Now, you can tag a node with a taxonomy term in the taxonomy reference field,
and that node will bubble to the very top of search results for that term.