<?php
/**
 * @file
 * Google Civic Contest Candidate Template
 *
 * Available variables:
 * - $title: The title for the fieldset
 * - $candidates: An array of candidates
 */
?>

<fieldset class='collapsible collapsed google-civic-candidate'>
  <legend><span class='fieldset-legend'><?php print $title; ?></span></legend>
  <div class='fieldset-wrapper'>
    <?php if (empty($candidates)): ?>
      <p><em>No candidates found.</em></p>
    <?php else: ?>
      <ul>
        <?php foreach ($candidates as $candidate): ?>
          <li class='google-civic-candidate'>
            <?php if (!empty($candidate->candidateUrl)): ?>
	 <p><a href=<?php echo substr(($candidate->candidateUrl), 0, 4) === "http" ? ($candidate->candidateUrl) : "http://" . ($candidate->candidateUrl); ?> target="_blank"><?php echo "$candidate->name - $candidate->party" ?></a></p>
            <?php else: ?>
              <p><?php print "{$candidate->name} - {$candidate->party}"; ?></p>
            <?php endif; ?>
          </li>
        <?php endforeach; ?>
      </ul>
    <?php endif; ?>
  </div>
</fieldset>
