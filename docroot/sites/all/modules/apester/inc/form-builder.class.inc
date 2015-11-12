<?php

class ApesterFormBuilder
{

    /**
     * Form elements to be created
     * @var array
     */
    protected $form = array();

    /**
     * @param string $name
     * @param string $title
     * @return ApesterFormBuilder $this
     */
    public function addFieldSet($name, $title)
    {
        $this->form[$name] = array(
            '#type' => 'fieldset',
            '#title' => t($title)
        );

        return $this;
    }

    /**
     * Creates new field
     * @param string $fieldset
     * @param string $fieldName
     * @param string $title
     * @param string $description
     * @param string $varName
     * @param string $type
     * @param bool $required
     * @return $this
     */
    public function addField($fieldset, $fieldName, $title, $description, $varName, $type = 'textfield', $required = true)
    {
        $this->form[$fieldset][$fieldName] = array(
            '#type' => $type,
            '#title' => $title,
            '#default_value' => variable_get($varName, ''),
            '#description' => $description,
            '#required' => $required,
        );

        return $this;
    }

    /**
     * Adds a submit button
     * @param string $name
     * @param string $value
     * @return $this
     */
    public function addSubmitButton($name, $value)
    {
        $this->form[$name] = array(
            '#type' => 'submit',
            '#value' => t($value)
        );

        return $this;
    }

    /**
     * Retrieves the Drupal form
     * @return array
     */
    public function getForm()
    {
        return $this->form;
    }

}