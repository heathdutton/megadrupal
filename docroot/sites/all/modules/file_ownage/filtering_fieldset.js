/**
 * @file
 */
(function ($) {

    /**
     * @file
     * UI addition to enhance the forms
     * - showing and hiding things based on what is slected nearby.
     *
     * @version 2012-03-13 based on the taxonomy_xml equivalent,
     * but updated to allow several selectors in the same page.
     *
     * USAGE:
     * <fieldset class="filtering-fieldset">
     *   <select class="filtering-selector">
     *     <option value='alpha'>Show The Alpha</option>
     *     <option value='beta'>Show Beta</option>
     *   </select>
     *
     *   <fieldset class="filtered-fieldset filtered-fieldset-alpha">
     *     Show this if alpha is selected.
     *   </fieldset>
     *
     *   <fieldset class="filtered-fieldset filtered-fieldset-beta">
     *     Show this if beta is selected.
     *   </fieldset>
     *
     * </fieldset>
     *
     * All the shown classes are required.
     * The class on the fieldset must match the value in the select option
     * plus the prefix.
     */

    Drupal.behaviors.filtering_fieldset = {
        attach : function(context) {
            console.log('Setting up filtered fieldset');
            $('.filtering-fieldset').addClass('filtered');
            $('.filtering-fieldset .filtering-selector')
            .addClass('filtering-trigger')
            .change(
                /*
                When the select changes, we change the class of the containing fieldset.
                and hide/show all the contents depending on what was selected.
                */
                function() {
                    $('.filtered-fieldset', $(this).closest('.filtering-fieldset')).hide().removeClass('filter-selected');
                    console.log('filtering to show ' + $(this).val());
                    if ($(this).val()) {
                        $('.filtered-fieldset-' + $(this).val(), $(this).closest('.filtering-fieldset')).show().addClass('filter-selected');
                    }
                }
            );
            // Trigger the filter to update the current display.
            $('.filtering-fieldset .filtering-selector').trigger('change');
            console.log('Fieldset filtered');
        }
    }

})(jQuery);
