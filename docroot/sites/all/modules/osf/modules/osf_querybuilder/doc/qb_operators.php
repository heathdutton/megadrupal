<p>
  The <strong>Operator Treatment</strong> panel enables you to test the
  effect of the default operator understood by the system. The default
  operator is what is the assumed operator between terms when a multi-term
  query is entered without specifically using an operator. For example, the
  query 'breast cancer symptoms' may be interpreted by the system as either
  'breast AND cancer AND symptoms' or 'breast OR cancer OR symptoms'
  depending on what default operator is chosen.
</p>
<p>
  The AND operator means that <strong>ALL</strong> terms in the query must
  be met. In set theory parlance, this means the
  <em><strong>intersection</strong></em> of results amongst all entered
  queries. This makes AND queries more restrictive (fewer in number) and
  more precise. The OR operator means <strong>ANY</strong> term in the
  query (see further elaboration below) will cause a positive result to be
  returned. In set theory parlance, this means the
  <em><strong>union</strong></em> of results amongst all entered queries.
  This makes OR queries more inclusive (greater in number) and less
  precise.
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> if your query includes a specific AND or OR
  operator within it, that specification overrides the default behavior.
</div>
<p>
  When the <strong>Operator Treatment</strong> panel is first opened, it
  shows the AND operator as the initial default selection as the default
  operator:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_operator_and.png" width="800" />
</p>
<p>
  However, if you pick OR in the dropdown list, you also get another choice
  presented, called the <strong>OR criterion</strong>:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_operator_or.png" width="800" />
</p>
<p>
  By virtue how you set this <strong>OR criterion</strong>, you can direct
  OR to be treated strictly as the union treatment noted above, or you may
  actually cause the OR operator to work more as a "relaxed" AND, or any
  point on the spectrum in between. Here is how that works.
</p>
<p>
  In the <strong>OR criterion</strong> box, you have a number of options
  for how to specify the criterion.
</p>
<p>
  Specification strings may have the following formats:
</p>
<p>
  Generally speaking, lower integer numbers or lower percentages cause the
  OR operator to act in the more standard, inclusive way (more results);
  higher integer numbers or higher percentages cause the OR operator more
  like AND with fewer, more restricted results.
</p>