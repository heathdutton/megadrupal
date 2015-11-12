from dcomp import DRecord, ComputingApplication, DCommand
import sys

__author__ = 'Daniel Zhou'


class CheckPython(DCommand):

    def execute(self):
        # set agent name.
        self.result['agent'] = self.config.get_agent_name()

        # python is always installed.
        self.result['python'] = {'title': 'Python', 'version': sys.version, 'installed': True}

        # numpy (http://www.numpy.org/)
        try:
            import numpy
            self.result['numpy'] = {'title': 'NumPy', 'version': numpy.__version__, 'installed': True}
        except ImportError:
            self.result['numpy'] = {'title': 'NumPy', 'installed': False}

        # scipy (http://www.scipy.org/)
        try:
            import scipy
            self.result['scipy'] = {'title': 'SciPy', 'version': scipy.__version__, 'installed': True}
        except ImportError:
            self.result['scipy'] = {'title': 'SciPy', 'installed': False}

        # matplotlib (http://matplotlib.org/)
        try:
            import matplotlib
            self.result['matplotlib'] = {'title': 'matplotlib', 'version': matplotlib.__version__, 'installed': True}
        except ImportError:
            self.result['matplotlib'] = {'title': 'matplotlib', 'installed': False}

        # Orange (http://orange.biolab.si/) no official Python3 support as of now.
        orange_data = {'title': 'Orange', 'installed': False}
        try:
            import Orange
            orange_data['version'], orange_data['installed'] = Orange.version.version, True
        except ImportError:
            pass
        self.result['orange'] = orange_data

        # rpy2 (http://rpy.sourceforge.net/) python-R interface
        self.result['rpy2'] = {'title': 'rpy2', 'installed': False}
        try:
            import rpy2
            self.result['rpy2']['version'], self.result['rpy2']['installed'] = rpy2.__version__, True
        except ImportError:
            pass

        # igraph (http://igraph.org/python/)
        d = {'title': 'igraph', 'installed': False}
        try:
            import igraph
            d['version'], d['installed'] = igraph.__version__, True
        except ImportError:
            pass
        self.result['igraph'] = d

        # sciket-learn (http://scikit-learn.org/)
        d = {'title': 'scikit-learn', 'installed': False}
        try:
            import sklearn
            d['version'], d['installed'] = sklearn.__version__, True
        except ImportError:
            pass
        self.result['sklearn'] = d


    def prepare(self, params):
        pass


if __name__ == '__main__':
    record = DRecord(application='computing', command='check_python', label='Check Python Libraries')
    app = ComputingApplication()
    app.run_once(record)