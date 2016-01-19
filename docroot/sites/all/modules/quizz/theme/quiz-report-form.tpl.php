<?php
/**
 * @file
 * Themes the question report
 *
 * Available variables:
 * $form - FAPI array
 *
 * All questions are in form[x] where x is an integer.
 * Useful values:
 * $form[x]['question'] - the question as a FAPI array(usually a form field of type "markup")
 * $form[x]['score'] - the users score on the current question.(FAPI array usually of type "markup" or "textfield")
 * $form[x]['max_score'] - the max score for the current question.(FAPI array of type "value")
 * $form[x]['response'] - the users response, usually a FAPI array of type markup.
 */
?>
<h2><?php print t('Question results'); ?></h2>

<div class="quiz-report">
  <?php foreach ($form as $key => $sub_form): ?>
    <?php if (!is_numeric($key) || isset($sub_form['#no_report'])): ?>
      <?php continue; ?>
    <?php endif; ?>
    <?php unset($form[$key]); ?>

    <div class="quiz-report-row clearfix">
      <div class="quiz-report-question dt">
        <div class="quiz-report-question-header clearfix">
          <?php print drupal_render($sub_form['score_display']); ?>
          <h3><?php print t('Question') ?></h3>
        </div>

        <?php if (isset($sub_form['question'])): ?>
          <?php print drupal_render($sub_form['question']); ?>
        <?php endif; ?>
      </div>

      <?php if (isset($sub_form['response'])): ?>
        <div class="quiz-report-response dd">
          <h3 class="quiz-report-response-header"><?php print t('Response') ?></h3>
          <?php print drupal_render($sub_form['response']); ?>
        </div>
      <?php endif; ?>

      <div class="quiz-report-question-feedback dd">
        <?php print drupal_render($sub_form['question_feedback']); ?>
      </div>

      <div class="quiz-report-score-feedback dd">
        <?php print drupal_render($sub_form['score']); ?>
        <?php print drupal_render($sub_form['answer_feedback']); ?>
      </div>
    </div>
  <?php endforeach; ?>

  <div class="quiz-report-quiz-feedback dd">
    <?php if (isset($form['quiz_feedback']) && $form['quiz_feedback']['#markup']): ?>
      <h2><?php print t('Quiz feedback'); ?></h2>
      <?php print drupal_render($form['quiz_feedback']); ?>
    <?php endif; ?>
  </div>
</div>

<div class="quiz-score-submit">
  <?php print drupal_render_children($form); ?>
</div>
