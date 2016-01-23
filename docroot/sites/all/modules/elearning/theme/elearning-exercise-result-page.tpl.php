<?php

/**
 * @file
 * Template file for the exercise result page.
 *
 * The following variables can be used:
 *
 * - $exercise: the exercise object.
 * - $exercise_result: the exercise result object.
 * - $score: formatted score
 * - $question_results: a view containing the question results. The view can be
 *   edited.
 * - $status: Whether the exercise was passed or failed.
 *
 * Theme suggestions:
 * elearning-exercise-result-page--$exercise_type.tpl.php.
 */

?>
<div id="result-<?php print $exercise_result->result_id; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
<div class="result-table"><?php print $question_results; ?>
</div>
