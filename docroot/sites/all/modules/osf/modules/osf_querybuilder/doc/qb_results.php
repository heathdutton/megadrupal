<p>
  The results listing appears at the bottom of the expand/collapse panels
  discussed above. The results listing is one of the major benefits of the
  Query Builder in that it provides the full structural detail for each
  result and also displays its internal search score, which is shown at the
  upper right of each individual result record.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> in an actual results presentation, these
  structured values may be included or not, snippets might be provided,
  titles could be made active links, images or maps could be displayed
  depending on the type of data, etc. The QB merely shows the component
  building blocks presently available in the datasets for such displays.
</div>
<p>
  The results listing itself begins with a summary of the count for the
  results set produced by the various filters and query specifications in
  the Query Builder. These results are then paginated, in groups of ten,
  across potentially multiple results pages.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> when looking at scoring impacts from boosts, it is
  often helpful to compare the first and last pages of paginated results.
</div>
<p>
  The basic appearance of the results listing, showing an example first
  result, appears as follows:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_results.png" width="800" />
</p>
<p>
  The results section begins with the standard help icon [<img src="<?php echo $_GET['path']; ?>/doc/images/qb_help.png" />]
  and the number display setting and paginator, as well as showing the
  total number of results for the current query and datasets. Then, each
  results listing follows a similar display format.
</p>
<p>
  The number display setting determines the number of search results
  displayed, with a default of <strong>50</strong>. You may also set this
  to other values up to <strong>100</strong>.
</p>
<p>
  The paginator automatically adjusts to the result count setting picked.
</p>
<p>
  The first line of every result shows the record's title
  (<code>prefLabel</code>), what type of record it is as shown in
  <em>(italicized parentheses)</em>, and its search score at far right.
  Then, all structured data and metadata are presented below, with the
  attribute name shown in bold followed by its value. Note that if you
  mouseover any bolded property label (such as <code>prefLabel</code>) you
  will see a tooltip showing you the URI leading to the specification for
  that property.
</p>
<p>
  Most values are string text, but some are URLs, in which case a live link
  is presented. Some of the attributes or properties, too, also have
  records within one of the governing ontologies. <strong>Subject</strong>
  is a useful property to inspect in this manner. These are shown as a live
  link as well; clicking on them will take you to a structured data
  listing, in this example for the <strong>Subject</strong> of "breast":
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_subject_details.png" />
</p>
<p>
  This record then presents all of its respective structured information.
  Via this mechanism, it is possible to explore the structural aspects of
  some properties as contained in the governing ontologies.
</p>
<p>
  Then, at the end of the structured data listing the dataset to which each
  particular result belongs is shown. This presentation is repeated for all
  records in the results set.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> by convention, if a given result or record lacks a
  value for a given attribute, that attribute is not listed. Thus, there
  are no "null" attributes shown in the display.
</div>
<p>
  Also, to keep display length manageable, the full body of the content
  (the "content body" attribute) is <strong>NOT</strong> shown for each
  record. To see the full body text, it is necessary to click on the
  record's URL link.
</p>