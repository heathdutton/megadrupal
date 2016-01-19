<div id="anki-ui-finished"><?php print t('Congratuations!') ?></div>
<p><?php print t('You have finished for now.'); ?></p>
<p><?php
  // TODO: Show this data again if we can get it!
  //print t('At this time tomorrow:<br />There will be <span class="anki-ui-count">!reviews_count reviews</span>.<br />And <span class="anki-ui-count">!new_count new</span> cards.', 
  //  array('!reviews_count' => $reviews_count, '!new_count' => $new_count));
  print t('<strong>Please return around this time tomorrow and review again!</strong> We only show you the cards that need to be reviewed each day.');
?></p>
<p><?php
  print t('<strong>Or you can <a href="!url">start a "cram session"</a>,</strong> and review random words regardless of when they are due.', array('!url' => url('anki/review')));
?></p>

