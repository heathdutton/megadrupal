<p>
  The <strong>Attributes Restrictions and Boosting</strong> panel allows
  you to filter search results by attribute and to change boost values to
  affect the rankings of the results set. The attribute filters work in
  conjunction with any other specifications that have been made above,
  namely simple or advanced search.
</p>
<p>
  Like the other panels, clicking on the Attributes panel causes it to
  expand; clicking on the expanded header causes it to collapse. Also, like
  all other panels, contextual help is provided via the icon button at the
  upper right of the panel[<img src="<?php echo $_GET['path']; ?>/doc/images/qb_help.png" />].
  Here is the basic view of this panel:
</p>
<p>
  The attributes combobox works similarly to what was described for
  Advanced Search. Via autocompletion and the entry of a few characters,
  the listing of possible attributes to select from gets narrowed. Further,
  only those attributes that meet the dataset and query selections made
  elsewhere appear on this list.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> invoking a 'Search' is necessary to narrow
  available attribute and value options.
</div>
<p>
  Any attribute selected in this panel <strong>MUST</strong> exist in a
  given record in order for that result to appear in the results list.
  Thus, any entry, no matter the boosting weight assigned, must appear for
  a positive search hit. However, also note that <strong>ANY</strong> value
  for that attribute will satisfy the condition and be acceptable. (For
  actual value filtering, see next section.)
</p>
<p>
  Once a filter is selected, it is then possible to indicate its relative
  weight in the results set ranking order. You do this by changing the
  value at the right of the entry line.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> by convention, all initial boost weights are set
  to 1.0. Thus, a value greater than 1.0 means the attribute is to be
  ranked higher; a value less than 1.0 means the attribute is to be ranked
  lower than standard.
</div>
<div class="boxYellowSolid">
  <strong>Note:</strong> the actual algorithm to construct a "score" for a
  given result can be quite complicated in the search function. As a
  result, it is often hard to see a commensurate change in score via the
  boost, though a numeric impact will occur and rank order will most likely
  change.
</div>
<p>
  The plus (+) button allows you to enter another filter or weighting
  boost. You may add as many of these filters as you wish.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> to test the effect of boost differences, first run
  a search without boost and inspect the resulting search scores as
  described under <strong>Results Listing</strong> below. Then, run again
  with the boosts and inspect score and ranking differences.
</div>
<p>
  At the bottom of the panel, a search may be invoked via the 'Search'
  button or all QB settings may be 'Clear' back to original settings. These
  buttons may be applied here, on this panel, or via the same buttons on
  the top-line search input.
</p>