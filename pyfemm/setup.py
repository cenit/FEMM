from setuptools import setup

setup(
    name='pyfemm',
    version='0.1.4',
    author='David Meeker',
    author_email='dmeeker@ieee.org',
    packages=['femm'],
    url='http://www.femm.info/wiki/pyFEMM',
    license_files = ('LICENSE.txt',),
    description='Python interface to Finite Element Method Magnetics (FEMM)',
    long_description=open('README.txt').read(),
    install_requires=[
		'pywin32 >= 1.0 ; platform_system=="Windows"',
		'matplotlib',
		'six'
    ],
	classifiers=[
		"DDevelopment Status :: 4 - Beta",
		"Intended Audience :: Science/Research",
		"Operating System :: Microsoft :: Windows",
		"Topic :: Scientific/Engineering :: Electronic Design Automation (EDA)",
		"License :: Aladdin Free Public License (AFPL)",
    ],
)
