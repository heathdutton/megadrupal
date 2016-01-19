<p>
  The Query Builder-Search version (QB) is a generalized search utility for
  querying the datasets within the OSF. The QB
  has three purposes:
</p>
<ol>
  <li>It is a basis for learning about the structured data within a OSF instance
  </li>
  <li>It is a testbed for exploring the effect of attribute and value
  boostings on the ranking order of search results sets, and
  </li>
  <li>It is a means to refine search queries and then generate query code
  that can be embedded into Web pages and templates.
  </li>
</ol>
<p>
  The Query Builder is an independent, JavaScript application written in
  JQuery.
</p>
<h3 id="QueryBuilder-Search-GeneralOverviewandOperation">
  General Overview and Operation
</h3>
<p>
  The QB consists of a top-line search box, followed below by a series of
  expand-and-collapse panels. Specialized actions take place within these
  panels. The operations of each of these panels is described below. Each
  panel also has a help icon [<img src="<?php echo $_GET['path']; ?>/doc/images/qb_help.png" />],
  which when clicked brings up contextual help for that panel.
</p>
<p>
  Upon invoking a search, a results listing appears at the bottom of the
  standard panels. Please make sure and review the <strong>Results
  Listing</strong> section near the bottom since how results are presented
  in the query builder is one of its benefits.
</p>
<p>
  The overall screenshot for the Query Builder is as follows:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_main.png" width="800" />
</p>
<p>
  with a results set appearing underneath:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_main_results.png" width="800" />
</p>
<h3 id="QueryBuilder-Search-SearchBoxandInput">
  Search Box and Input
</h3>
<p>
  Any standard site query may be entered into the search box. For legal and
  available query syntax.
</p>
<p>
  A search is conducted upon invoking the 'Search' button. The resulting
  results set is then listed at the bottom of the page in scoring rank
  order. A 'Clear' causes all settings throughout the QB to be returned to
  its default (start-up) settings, as if the application were reloaded. The
  one exception is that if a query had been entered into the top-line
  search box, it is kept.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> these same 'Search' and 'Clear' operations apply
  when the buttons are encountered on other panels.
</div>
<p>
  The gear icon [<img src="<?php echo $_GET['path']; ?>/css/ui-lightness/images/cog_add.png" />] at
  far right turns <a href="http://techwiki.openstructs.org/index.php/Inference_Concept">inferencing</a>
  on or not. (Think of "inferencing" as resulting in a kind of query
  expansion, by including the children of a given attribute.) By default,
  inferencing is on. To test the effect of inferencing, conduct the same
  search with inferencing on or not and select the 'type' attribute in one
  of the lower panels. When inferencing is on, generally the results set is
  much larger since matches to related (inferred) concepts are brought into
  the results.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> inferencing is limited to attributes that are also
  classes. These are marked with a trailing asterisk "*" in the attributes
  dropdown listing. Likely the two attributes of most interest for
  inference testing include <strong>type</strong> and
  <strong>Subject</strong>.
</div>
<div class="boxYellowSolid">
  <strong>Note:</strong> if you do not enter a query within the search
  entry box, a 'Search' will bring up a results set containing the records
  of all active datasets.
</div>