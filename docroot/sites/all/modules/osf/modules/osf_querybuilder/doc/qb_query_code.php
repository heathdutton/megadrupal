<p>
  The <strong>Search Query Code</strong> panel presents the internal query
  specification resulting from any of the prior search, dataset selection,
  filter selection or boosting factors. The code that is presented here may
  be copied by developers for Web page or layout template use (see
  <strong>Future Directions</strong> below). The code in this panel is also
  a good way to understand the various components in your queries.
</p>
<p>
  Here is an example screen showing one of these code samples:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_query_code.png" width="800" />
</p>
<p>
  Some of the items you might see in this listing and what they mean are:
</p>
<ul>
  <li>
    <code><span>query</span></code> -- the base query used
  </li>
  <li>
    <code><span>page</span></code> -- paginator instructions for the
    current page; may ignore
  </li>
  <li>
    <code><span>items</span></code> -- paginator instructions for number of
    items per page; may ignore
  </li>
  <li>
    <code><span>enableInference</span></code> -- inference indicator
  </li>
  <li>
    <code><span>datasetFilter</span></code> -- the URI of a dataset to
    include
  </li>
  <li>
    <code><span>attributeValueBoost</span></code> -- the URI of an
    attribute and its associated value to boost, including its assigned
    weight
  </li>
  <li>
    <code><span>datasetBoost</span></code> -- the URI of a dataset to
    boost, including its assigned weight
  </li>
  <li>
    <code><span>searchRestriction</span></code> -- the URI of an attribute
    to restrict, including its assigned weight
  </li>
</ul>
