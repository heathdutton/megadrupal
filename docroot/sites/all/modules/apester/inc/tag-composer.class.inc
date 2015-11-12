<?php

class ApesterTagComposer {

    /**
     * composes interaction tag with given id.
     * @param $interactionId
     * @return string
     */
    public function composeInteractionTag($interactionId)
    {
        return '<interaction id="' . $interactionId . '"></interaction>';
    }

    /**
     * composes automation tag (with userId taken by token)
     * @return string
     */
    public function composeAutomationTag()
    {
        $userId = $this->getUserId();

        if (!$userId) {
            return '';
        }

        return '<interaction data-automation="' . $userId . '"></interaction>';
    }

    public function composeEditorSuggestionsTag($authToken)
    {
        return '<iframe id="apester-field-suggestions" src="' . APESTER_EDITOR_BASE_URL . '/#/editor-suggestions/drupal?access_token=' . $authToken . '" width="100%" border="0"></iframe>';
    }

    /**
     * Returns the user ID according to the given Apester Token (in settings panel)
     * @return mixed|void
     */
    private function getUserId()
    {
        $userId = variable_get(APESTER_USER_ID_VAR, null);

        if (!$userId) {
            $userId = $this->fetchUserId($this->getAuthToken());
            variable_set(APESTER_USER_ID_VAR, $userId);
        }

        return $userId;
    }

    /**
     * Returns the publisher token from the DB.
     * @return string
     */
    private function getAuthToken() {
        return variable_get(APESTER_AUTH_VAR, null);
    }

    /**
     * Fetches the user ID from the Apester's user service and returns the user ID.
     * @param string $token
     * @return string
     */
    private function fetchUserId($token) {
        $response = json_decode(file_get_contents(APESTER_USER_SERVICE_BASE_URL . '/user?authToken=' . $token, false, $this->createStreamContext()));

        if (!$response->code == 200 || !$response->payload) {
            return '';
        }

        return $response->payload->userId;
    }

    /**
     * Creates stream context that ignores errors on HTTP calls
     * @return resource
     */
    private function createStreamContext() {
        return stream_context_create(array(
            'http' => array('ignore_errors' => true)
        ));
    }
}