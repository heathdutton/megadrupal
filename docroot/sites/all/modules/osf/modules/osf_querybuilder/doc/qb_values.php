<p>
  The <strong>Values Boosting</strong> panel allows you to boost values to
  affect the rankings of the results set. It will not act to do any
  filtering whatsoever; for that purpose, see the Attributes panel above.
  The values boosts work in conjunction with any other specifications that
  have been made above, namely simple or advanced search or attribute
  boosting or filtering.
</p>
<p>
  Like the other panels, clicking on the <strong>Values Boosting</strong>
  panel causes it to expand; clicking on the expanded header causes it to
  collapse. Also, like all other panels, contextual help is provided via
  the icon button at the upper right of the panel [<img src="<?php echo $_GET['path']; ?>/doc/images/qb_help.png" />].
  Here is the basic view of this panel:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_values.png" width="800" />
</p>
<p>
  The first combobox presented enables you to select which attribute you
  want to select for boosting values. The attributes themselves are of two
  types: There are free text entries [<img src="<?php echo $_GET['path']; ?>/css/ui-lightness/images/text_smallcaps.png" />]
  and controlled vocabulary [<img src="<?php echo $_GET['path']; ?>/css/ui-lightness/images/anchor.png" />]
  entries, with these icons appearing in the adjacent values dropdown
  combobox. For free text entries, any term or string may be entered into
  this box; for controlled vocabulary entries, you must pick from one of
  the available options, which are presented to you via a dropddown list,
  also shown with an autocompletion icon [<img src="<?php echo $_GET['path']; ?>/doc/images/qb_autocomplete.png?" />].
  Like other autocompletion fields, typing more characters causes the
  possible entries to be narrowed to those that match the string. A good
  attribute to try this feature with is Subject, since there is a long list
  of subjects within the available ontologies.
</p>
<p>
  Once an attribute value is selected, it is then possible to indicate its
  relative weight in the results set ranking order. You do this by changing
  the value at the right of the entry line.
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
<p>
  For a given document or result to receive this scoring boost, it must
  both have the applicable attribute AND the specific value you specified.
  Without both of those conditions being met, the record receives no
  scoring boost and its overall score is unaffected. (However, its absolute
  rank order might be lowered if other records or documents receive the
  boost causing them to appear higher in the results listing.)
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