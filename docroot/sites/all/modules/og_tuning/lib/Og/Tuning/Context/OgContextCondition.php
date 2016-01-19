<?php

namespace Og\Tuning\Context;

class OgContextCondition extends \context_condition
{
    public function condition_values()
    {
        return array(1 => t('Always active'));
    }

    public function editor_form($context = NULL)
    {
        $form = parent::editor_form($context);

        $form[1]['#title'] = t('Always active');
        $form['#weight'] = -10;

        return $form;
    }

    public function execute($value)
    {
        if ($ogContext = og_context()) {
            $this->condition_met($context, $ogContext);
        }
    }
}
