<?php

class DevControl_Graph_Node
{
    /**
     * Path caching. This will cache relative path to any destination it has
     * been asked for looking, even if not the first node.
     * 
     * @var array
     */
    public $relativePath = array();

    /**
     * @var string
     */
    public $name;

    /**
     * @var array
     */
    public $links = array();

    /**
     * This boolean will be incremented when the current node is lookup for
     * a particular destination, and will be decremented once done. When
     * looking up into the wrong direction, we may end up by caching a false
     * negative, we don't want that.
     * 
     * @var bool
     */
    public $beingParsed = 0;

    protected function _parseLinks($destination)
    {
        $found = $tainted = $this->relativePath[$destination] = false;

        ++$this->beingParsed;

        foreach ($this->links as $link) {
            $node = $link->to;

            if ($destination == $node->name) {
                // Check for direct neighbour.
                $found = new DevControl_Graph_Path(array($link));
                break;
            } else if ($node->beingParsed > 0) {
                // Do not cache when in circular dependency.
                $tainted = true;
                continue;
            } else if ($newPath = $node->find($destination)) {
                // Normal recursion.
                $newPath->prepend($link);
                if (!$found || ($found->compareTo($newPath) > 0)) {
                    $found = $newPath;
                }
            }
        }

        --$this->beingParsed;

        if (!$found && $tainted) {
            // This is not the proper way, I assume. But it ensures that we
            // dont cache false negative in case of circular dependency.
            $this->relativePath[$destination] = null;
        } else {
            $this->relativePath[$destination] = $found;
        }
    }

    /**
     * Find the shorter path to the asked node if it exists.
     * 
     * This implements a simple naïve algorithm that will test every possible
     * path combination using a recursive algorithm.
     * 
     * @param string|DevControl_Graph_Node $destination
     *   Node to find.
     * 
     * @return DevControl_Graph_Path
     *   Found path. false if path does not exists.  
     */
    public function find($destination)
    {
        if ($destination instanceof DevControl_Graph_Node) {
            $destination = $destination->name;
        }
        if (!isset($this->relativePath[$destination])) {
            $this->_parseLinks($destination);
        }
        return $this->relativePath[$destination] ? clone($this->relativePath[$destination]) : false;
    }

    /**
     * Add new link.
     * 
     * @param string|DevControl_Graph_Node|DevControl_Graph_Link $link
     */
    public function addLink($link)
    {
        if ($link instanceof DevControl_Graph_Link) {
            $link->from = $this;
            $this->links[] = $link;
        } else if ($link instanceof DevControl_Graph_Node) {
            $this->links[] = new DevControl_Graph_Link($this, $link);
        } else {
            $this->links[] = new DevControl_Graph_Link($this, new DevControl_Graph_Node($link));
        }
    }

    /**
     * Default constructor.
     * 
     * @param string $name
     * @param array $links
     *   Array of values eligible for the DevControl_Graph_Node::addLink() method.
     */
    public function __construct($name, array $links = array())
    {
        $this->name  = $name;
  
        foreach ($links as $link) {
            $this->addLink($link);
        }
    }
}
