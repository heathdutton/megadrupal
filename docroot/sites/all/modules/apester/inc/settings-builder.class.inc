<?php

class ApesterSettingsBuilder
{

    /**
     * @type ApesterFormBuilder
     */
    protected $formBuilder;

    /**
     * Constructor function for the apester settings builder class
     * @param ApesterFormBuilder $formBuilder
     */
    public function __construct($formBuilder)
    {
        $this->formBuilder = $formBuilder;
    }

    /**
     * Build and retrieve the form
     * @return array
     */
    public function build()
    {
        $this->buildAuthenticationForm();
        $this->formBuilder->addSubmitButton('submit', 'Submit');

        return $this->formBuilder->getForm();
    }

    private function buildAuthenticationForm()
    {
        $this->formBuilder
            ->addFieldSet('authentication', 'Authentication')
            ->addField('authentication', 'auth_token', 'Authentication Token', 'Add your Apester Authentication Token taken from Qmerce.com here.', APESTER_AUTH_VAR);

        return $this;
    }
}
