# -- Imports -----------------------------------------------------------------

# -- Path setup --------------------------------------------------------------

# -- Project information -----------------------------------------------------
project = 'ES Client API FreeRTOS'
copyright = 'EnduroSat'
author = 'Iliya Iliev'

# -- Constants ---------------------------------------------------------------

# -- General configuration ---------------------------------------------------
# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx_rtd_theme',
    'myst_parser',
    'sphinx_c_autodoc',
    'sphinx_c_autodoc.napoleon',
    'sphinx_c_autodoc.viewcode',
    "sphinx_design",
    "sphinx_git",
    "sphinx_inline_tabs",
    "sphinx_copybutton",
    "sphinxcontrib.mermaid"
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

# -- API C files -------------------------------------------------------------
c_autodoc_roots = [
    '../libs/esclient-api/include/',
    '../libs/esclient-api/src/'
]

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages. See the documentation for
# a list of builtin themes.
html_theme = "sphinx_rtd_theme"

autosectionlabel_prefix_document = True
html_theme_options = {
    'logo_only': False,
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': True,
    'vcs_pageview_mode': '',
    'style_nav_header_background': 'rgba(52,49,49,1) 100%;',
    'collapse_navigation': False,
    'sticky_navigation': True,
    'navigation_depth': 3,
    'includehidden': True,
    'titles_only': True,
}

version = "v1.0.0"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_show_sourcelink = False
html_title = "ES Client API FreeRTOS"
html_static_path = ['_static']
html_logo = "_static/es_logo.png"
