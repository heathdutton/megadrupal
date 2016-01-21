<p>
  The <strong>Advanced Search</strong> panel allows quite complicated
  queries to be built. These extensions are added to whatever query already
  exists in the top-line search box. If there is no query in that search
  box, then the specifications in this panel apply on their own.
</p>
<p>
  Like the other panels, clicking on it causes it to expand; clicking on
  the expanded header causes it to collapse. Also, like all other panels,
  contextual help is provided via the icon button at the upper right of the
  panel[<img src="<?php echo $_GET['path']; ?>/doc/images/qb_help.png" />].
</p>
<p>
  When first expanded, a single line of new query expansion is presented.
  This line, from left to right, has an operator box, followed by an
  attribute selection dropdown, and then a means to assign a value to that
  attribute in the next box. At the end of the line a [+] button allows a
  new line to be added for additional query expansion. If the [-] button is
  selected, the current line is removed. The basic aspect of this panel is
  as shown:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_advanced_search.png" width="800" />
</p>
<p>
  The operator box enables multiple query expansion lines to be combined
  together or nested. The available operators are AND, OR or NOT, in
  combination or not with parentheses if nesting is also desired. They are
  selected via this dropdown list:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_operators.png" />
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> the first and last operator boxes shown are
  limited to the open ("(") and close (")") parentheses, respectively.
</div>
<div class="boxYellowSolid">
  <strong>Note:</strong> an improperly specified or missing operator box
  will show in red when attempting to invoke a search. The operator must be
  corrected before the search will be accepted for processing. Here is how
  this error looks:
</div>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_bad_operator.png" />
</p>
<p>
  For a single expansion line, no operator needs to be designated in this
  first operator box. Thereafter, an operator must be specified to "chain
  together" more query expansion lines.
</p>
<p>
  Once the operator is selected (or not, for a single line), it is next
  necessary to select one of the available attributes. These appear as a
  dropdown list in the next box. This attribute box features
  autocompletion, so beginning to type a few letters will match those same
  letters in the attribute titles no matter where the matching string may
  occur (beginning, middle or end of a word). As more letters get typed
  into the combobox entry, the selections get fewer.
</p>
<div class="boxYellowSolid">
  <strong>Note</strong>: autocompletion typically starts once you place the
  cursor into the applicable text or combobox.
</div>
<div class="boxYellowSolid">
  <strong>Note</strong>: autocompletion first tries to complete the search
  words from the start of the word. So, "can" would return all the results
  that starts with "can" as a word. However, if you want to find "can"
  anywhere in the word, simply add the wildcard asterisk character [ * ] in
  front of it, such as <code>*can</code>.
</div>
<p>
  The available attributes to select from also may be reduced in number
  based on prior selections or a query already entered into the top-line
  search box. That is, the attributes available for selection at any given
  step are limited to those that are active for the query that has been
  built to that point, with the narrowing occurring each time the 'Search'
  button is invoked without an intervening 'Clear'.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> an attribute in the listing that is followed by
  the [ + ] character means that the given attribute is not defined within
  an ontology. As noted above, an attribute with a trailing asterisk may be
  used for inferencing.
</div>
<div class="boxYellowSolid">
  <strong>Note</strong>: invoking a 'Search' is necessary to narrow
  available attribute and value options.
</div>
<p>
  Thus, very complicated query constructions may result in very few
  attributes being available for further filtering.
</p>
<p>
  The attributes themselves are of two types: There are free text entries
  [<img src="<?php echo $_GET['path']; ?>/css/ui-lightness/images/text_smallcaps.png" />]
  and controlled vocabulary [<img src="<?php echo $_GET['path']; ?>/css/ui-lightness/images/anchor.png" />]
  entries, with these icons appearing in the adjacent values dropdown
  combobox. For free text entries, any term or string may be entered into
  this box; for controlled vocabulary entries, you must pick from one of
  the available options, which are presented to you via a dropddown list,
  also shown with an autocompletion icon [<img src="<?php echo $_GET['path']; ?>/doc/images/qb_autocomplete.png" />].
  Like other autocompletion fields, typing more characters causes the
  possible entries to be narrowed to those that match the string. A good
  attribute to try this feature with is <strong>Subject</strong>, since
  there is a long list of subjects within the available ontologies.
</p>
<p>
  You may construct as many query expansion lines as you would like, again
  with the caveat that attribute choices will continue to narrow with more
  filters. If you want to back up a step, you can delete the entire line.
</p>
<p>
  At the bottom of the panel, a search may be invoked via the 'Search'
  button or all QB settings may be 'Clear' back to original settings. These
  buttons may be applied here, on this panel, or via the same buttons on
  the top-line search input.
</p>
<h4>
  Date Entry Fields
</h4>
<p>
  The four fields of <strong>date created</strong>, <strong>date modified
  (dcterms)</strong>, <strong>first created</strong> and <strong>last
  modified</strong> bring up the special date entry field:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_date_field.png" width="800" />
</p>
<p>
  Selecting one of these four brings up the to and from date entry widget.
  By picking the calendar icon [<img src="<?php echo $_GET['path']; ?>/doc/images/qb_calendar_icon.png" />],
  the calendar appears, that then enables you to pick your date that then
  gets entered in the proper date format. You repeat this for the ending
  date as well.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> there are a couple of other date-type fields that
  do <strong>NOT</strong> invoke this widget because they are free-form
  text string entry fields.
</div>
