<?php

/**
 * Simple class for a w3c_validator module Message.
 *
 * The aim of this class is to format the errors or warning Messages in a
 * similar way, no matter which validator as been employed.
 * NOTE : still, this is oriented to easily understand W3C Markup Validator
 * soap1.2 response format.
 *
 */
class W3Cvalidator_Message
{
    /**
     * Line corresponding to the message.
     *
     * Within the source code of the validated document, refers to the line
     * which caused this message.
     * @var int
     */
    public $line;

    /**
     * Column corresponding to the message.
     *
     * Within the source code of the validated document, refers to the column
     * within the line for the message.
     * @var int
     */
    public $col;

    /**
     * The actual message.
     * @var string
     */
    public $message;

    /**
     * Explanation for this message.
     *
     * HTML snippet which describes the message, usually with information on
     * how to correct the problem.
     * @var string
     */
    public $explanation;

    /**
     * Source which caused the message.
     *
     * the snippet of HTML code which invoked the message to give the
     * context of the e.
     * @var string
     */
    public $source;

    /**
     * Constructor for a response message
     *
     * @param object $node A dom document node.
     */
    function __construct($node = null)
    {
        if (isset($node)) {
            foreach (get_class_vars('W3Cvalidator_Message') as
                $var => $val) {
                $element = $node->getElementsByTagName($var);
                if ($element->length) {
                    $this->$var = $element->item(0)->nodeValue;
                }
            }
        }
    }
}

?>