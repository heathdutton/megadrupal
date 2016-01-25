<p>
  The <strong>Phrase Treatment</strong> panel provides two setting choices.
  The first setting is how the the system should identify a phrase. This
  identification is done by setting a distance parameter for how adjacent
  the terms must be to one another to qualify as a "phrase". A setting of
  "1", for example, tells the system the terms must be right next to one
  another. Higher numbers allow greater distance. A phrase distance of one
  would match on "breast cancer" for example, but would not match "cancer
  of the female breast".
</p>
<p>
  The second setting choice allows phrases that occur within specified
  field types (per the dropdown list) to be boosted in score when the
  phrase occurs. Like other boosting options, use of the [+] button allows
  a new field to be specified for getting a boosting factor. The boosting
  factor or weight is set separately for each field designated. The
  appearance of this panel is as follows:
</p>
<p>
  <img src="<?php echo $_GET['path']; ?>/doc/images/qb_phrases.png?" width="800" />
</p>
<div class="boxYellowSolid">
  <strong>Note:</strong> for the <strong>Phrase Treatment</strong> settings
  to work, all or a portion of the input query string must be designated
  within double quotes ("double quotes") to identify the phrase.
</div>