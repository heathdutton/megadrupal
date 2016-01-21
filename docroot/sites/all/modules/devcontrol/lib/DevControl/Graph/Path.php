<?php

class DevControl_Graph_Path
{
    /**
     * Global path weight
     * 
     * @var float
     */
    public $weight;

    /**
     * Array of ordered DevControl_Graph_Link instances
     * 
     * @var array
     */
    public $links = array();

    /**
     * Compare to
     * 
     * @param DevControl_Graph_Path $path
     * 
     * @return float
     *   A negative float, zero, or a positive float as this object is less
     *   than, equal to, or greater than the specified object.
     */
    public function compareTo(DevControl_Graph_Path $path)
    {
        return $this->weight - $path->weight;
    }

    /**
     * Append link.
     * 
     * @param DevControl_Graph_Link $link
     */
    public function append(DevControl_Graph_Link $link)
    {
        $this->links[] = $link;
        $this->weight += $link->weight;
    }

    /**
     * Prepend DevControl_Graph_Link
     * 
     * @param DevControl_Graph_Link $link
     */
    public function prepend(DevControl_Graph_Link $link)
    {
        array_unshift($this->links, $link);
        $this->weight += $link->weight;
    }

    /**
     * Default constructor.
     * 
     * @param array $path
     * @param float $weight = null
     */
    public function __construct(array $links = null)
    {
        if (isset($links)) {
            foreach ($links as $link) {
                $this->append($link);
            }
        }
    }

    public function __toString()
    {
        $path = array();

        foreach ($this->links as $link) {
            $path[] = $link->from->name . '->' . $link->to->name;
        }

        return '[' . $this->weight . '] ' . implode(', ', $path);
    }
}
