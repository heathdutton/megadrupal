<?php

class DevControl_Graph_Link
{
    /**
     * Default weight if none specified
     */
    const WEIGHT_DEFAULT = 1;

    /**
     * @var float
     */
    public $weight;

    /**
     * @var DevControl_Graph_Node
     */
    public $from;

    /**
     * @var DevControl_Graph_Node
     */
    public $to;

    /**
     * Compare to
     * 
     * @param DevControl_Graph_Path $path
     * 
     * @return float
     *   A negative float, zero, or a positive float as this object is less than,
     *   equal to, or greater than the specified object.
     */
    public function compareTo(DevControl_Graph_Link $link)
    {
        return $this->weight - $link->weight;
    }

    /**
     * Default constructor
     * 
     * @param DevControl_Graph_Node $node
     * @param float $weight = DevControl_Graph_Link::WEIGHT_DEFAULT
     */
    public function __construct(DevControl_Graph_Node $from, DevControl_Graph_Node $to,
        $weight = DevControl_Graph_Link::WEIGHT_DEFAULT)
    {
        $this->from   = $from;
        $this->to     = $to;
        $this->weight = $weight;
    }
}
