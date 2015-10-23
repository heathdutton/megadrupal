<?php

function theme_choices_choices($variables) {
  $items = $variables['element']['#choice_items'];
  $total_vote_count = '';
  if (isset($variables['element']['#total_count'])) {
    $total_vote_count = format_plural($variables['element']['#total_count'], 'Total of 1 vote', 'Total of @count votes');
  }
  
  $output = array();
  foreach ($items as $delta => $item) {
    $bar = '';
    if (isset($item['#percentage'])) {
      $percentage = round($item['#percentage']);
      $bar = "<div class='choice-percentage-bar' style='width: {$percentage}%'></div>";
    }
    $output[] = "<div class='choice-item'><div class='choice-data clearfix'>{$item['#markup']}$bar</div></div>";
  }
  
  $output[] = "<div class='choices-total-vote-count'>$total_vote_count</div>";
  
  return join('', $output);
}


function theme_choices_radiobtn($variables) {
  $entity_id  = intval($variables['element']['#entity_id']);
  $delta      = intval($variables['element']['#delta']);
  $vote_count = isset($variables['element']['#votes_count']) ? strip_tags($variables['element']['#votes_count']) : NULL;
  $txt        = strip_tags($variables['element']['#txt']);

  $output = "<input type='radio' class='choice' name='choice' id='pollim-$entity_id-$delta' value='$delta' entityid='$entity_id' />";
  $output .= "<label for='pollim-$entity_id-$delta'>";
  $output .= !empty($vote_count) ? "<span class='choice-votecount'> $vote_count</span>" : '';
  $output .= "$txt</label>";
  
  return $output;
}
