<p>
  Clicking on the <strong>Dataset</strong> panel causes it to expand. The
  panel then shows all datasets available on OSF:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_datasets.png" width="800" />
</p>
<p>
  Each of the datasets may be included or not in the content basis used by
  the Query Builder by checking or unchecking its associated checkbox.
  Alternatively, all datasets may be selected or not using the global links
  at the top of the panel.
</p>
<p>
  Each dataset shown also provides a count (in parentheses after the
  dataset title) for how many records it contains. If a query has already
  been entered into the top-line search box, these counts pertain to the
  number of record instances where the search query exists. If there is no
  query in the search box, the counts shown are for total numbers of
  records in each dataset.
</p>
<p>
  As datasets are selected, their total number appears in the panel header.
  Whatever dataset selections are made in this panel remain active during
  the current QB session or until you actively change them. Upon next
  startup of the QB, all available datasets are selected as active by
  default.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> for content inspections, make sure to restrict
  your datasets to the public content, also excluding the underlying
  ontologies and vocabularies.
</div>
