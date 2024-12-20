# Copyright (c) 2020 - 2024 Open Risk (https://www.openriskmanagement.com)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import math

from django import template
from django.template import engines
from django.utils.html import format_html
from django.utils.safestring import mark_safe

register = template.Library()


@register.filter
def get_flag(obj, attr):
    return obj[attr]


@register.simple_tag
def tuple_args(dictionary, a, b):
    if (a, b) in dictionary.keys():
        return math.sqrt(dictionary[(a, b)]/100000000)
    else:
        return 0


@register.filter()
def get_value(dictionary, key):
    if key in dictionary.keys():
        return int(10 * float(dictionary.get(key)))
    else:
        return 0


@register.filter()
def nbsp(value):
    if value:
        return mark_safe("&nbsp;".join(value.split(' ')))
    else:
        pass


@register.filter()
def slugify(value):
    tmp = value.lower()
    tmp = tmp.replace(" ", "_")
    return tmp


@register.filter()
def deslugify(value):
    tmp = value.replace("_", " ")
    return tmp


# INSTEAD OF {{ page.content | safe }}
@register.simple_tag()
def render_doc_page(value):
    django_engine = engines['django']
    my_template = django_engine.from_string(value)
    rendered = my_template.render()
    return rendered


# render a link to the Open Risk Manual
@register.simple_tag()
def orm(url, name):
    url_text = format_html('<a href="https://www.openriskmanual.org/wiki/' + url + '">' + name + '</a>')
    return url_text


# render a link to the Open Risk Blog
@register.simple_tag()
def blog(url, name):
    url_text = format_html('<a href="https://www.openriskmanagement.com/' + url + '">' + name + '</a>')
    return url_text


@register.filter()
def get_item(dictionary, key):
    return dictionary.get(key)


@register.filter()
def get_list_item(List, i):
    if i < len(List):
        return List[int(i)]
    else:
        return List[0]


@register.filter()
def significant_figures(x, s):
    return round(x, s)
