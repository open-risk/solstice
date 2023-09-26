# This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
# Copyright (c) 2022 - 2023 Open Risk (https://www.openriskmanagement.com)+
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

import json

from django.conf import settings
from django.http import HttpResponse
from django.template import RequestContext, loader
from django.views.generic import DetailView, ListView

from visualization_explorer.forms import VisualizationInteractiveForm
from visualization_explorer.models import Visualization

cgi_url = settings.CGI_URL


def Visualization_view(request, pk):
    """
    Interactive modification / calculation of Visualizations using Ajax calls

    **Context**

    ``Visualization``
        An instance of :model:`visualization_explorer.Visualization`.

    **Template:**

    :template:`visualization_explorer/Visualization_interactive.html`
    """

    # get the Visualization object
    visualization = Visualization.objects.get(pk=pk)
    context = RequestContext(request, {})

    t = None
    if visualization.visualization_data_mode == 0:
        # all-inclusive specification. we use a generic visualization template
        t = loader.get_template('visualization_explorer/visualization.html')
        spec = json.dumps(visualization.vega_specification)
        context.update({'object': visualization})
        context.update({'vega_specification': spec})
    elif visualization.visualization_data_mode == 1:
        # TODO fetch the actual data from the server via REST API
        # from the provided URL
        t = loader.get_template('visualization_explorer/visualization_single.html')
        spec = json.dumps(visualization.vega_specification)
        data = json.dumps(visualization.visualization_data)
        context.update({'object': visualization})
        context.update({'visualization_data': data})
        context.update({'vega_specification': spec})
    elif visualization.visualization_data_mode == 2:
        # spec + data mode (basic visualization)
        t = loader.get_template('visualization_explorer/visualization_single.html')
        spec = json.dumps(visualization.vega_specification)
        data = json.dumps(visualization.visualization_data)
        context.update({'object': visualization})
        context.update({'visualization_data': data})
        context.update({'vega_specification': spec})
    elif visualization.visualization_data_mode == 3:
        # spec + data mode (mashup visualization via REST)
        t = loader.get_template('visualization_explorer/visualization_mashup.html')
        spec = json.dumps(visualization.vega_specification)
        data = json.dumps(visualization.visualization_data)
        context.update({'object': visualization})
        context.update({'visualization_data': data})
        context.update({'vega_specification': spec})
    elif visualization.visualization_data_mode == 4:
        # spec + data mode (mashup visualization via local store)
        t = loader.get_template('visualization_explorer/visualization_mashup.html')
        spec = json.dumps(visualization.vega_specification)
        data = json.dumps(visualization.visualization_data)
        context.update({'object': visualization})
        context.update({'visualization_data': data})
        context.update({'vega_specification': spec})

    return HttpResponse(t.template.render(context))


class VisualizationList(ListView):
    model = Visualization
    template_name = 'visualization_explorer/visualization_list.html'
    redirect_field_name = 'redirect_to'

    def get_context_data(self, **kwargs):
        context = super(ListView, self).get_context_data(**kwargs)
        return context


class VisualizationObjectiveList(ListView):
    model = Visualization
    template_name = 'visualization_explorer/visualization_objective_list.html'
    redirect_field_name = 'redirect_to'

    def get_context_data(self, **kwargs):
        objectives = []
        for choice in Visualization.OBJECTIVE_CHOICE:
            objectives.append(choice[1])

        context = super(ListView, self).get_context_data(**kwargs)
        context.update({'objectives': objectives})
        return context


class VisualizationInteractive(DetailView):
    model = Visualization
    template_name = 'visualization_explorer/visualization_interactive.html'
    redirect_field_name = 'redirect_to'

    def get_context_data(self, **kwargs):
        visualization = super(VisualizationInteractive, self).get_object()
        context = super(DetailView, self).get_context_data(**kwargs)
        return context


def Visualization_interactive(request, pk):
    """
    Interactive modification / calculation of Visualizations using Ajax calls

    **Context**

    ``Visualization``
        An instance of :model:`visualization_explorer.Visualization`.

    **Template:**

    :template:`visualization_explorer/Visualization_interactive.html`
    """

    # get the Visualization object
    visualization = Visualization.objects.get(pk=pk)
    model_url = Visualization.model_url
    # serialize the Visualization
    Visualization_data = []
    response_data = ''
    log_data = ''
    results_data = ''
    # display the Visualization in a form
    formset = VisualizationInteractiveForm(instance=visualization)
    t = loader.get_template('visualization_explorer/visualization_interactive.html')
    context = RequestContext(request, {})
    context.update({'object': Visualization, 'formset': formset})
    context.update({'Visualization_data': Visualization_data})
    context.update({'results_data': results_data})
    context.update({'log_data': log_data})
    context.update({'model_url': model_url})
    return HttpResponse(t.template.render(context))
