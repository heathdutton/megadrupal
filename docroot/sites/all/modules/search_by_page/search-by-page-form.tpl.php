<?php

/**
 * @file
 * Default theme implementation for Search by Page search form.
 *
 * Available variables:
 * - $search_form: The complete search form ready for print.
 * - $search: Array of keyed search elements. Can be used to print each form
 *   element separately. This contains optional elements that may be added by
 *   other modules, and the following default elements:
 *   - $search['keys']: Text input field for search keywords.
 *   - $search['submit']: Form submit button.
 *   - $search['hidden']: Hidden form elements. Used to validate forms when
 *     submitted.
 * - $environment: The ID number of the search environment.
 * - $is_block: TRUE if the form is in the block, FALSE if in the page body.
 *
 * @see template_preprocess_search_by_page_form()
 */
?>
<div class="container">
  <?php print $search_form; ?>
</div>
