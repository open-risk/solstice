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

from django.http import Http404
from django.http import HttpResponse
from django.template import RequestContext, loader
from django.views.generic import ListView

from results_explorer.models import Calculation, ResultGroup


class ResultsList(ListView):
    model = Calculation
    template_name = 'results_explorer/calculation_list.html'

    # redirect_field_name = 'redirect_to'

    def get_context_data(self, **kwargs):
        context = super(ListView, self).get_context_data(**kwargs)
        return context


class ResultGroupList(ListView):
    model = ResultGroup
    template_name = 'results_explorer/result_group_list.html'

    # redirect_field_name = 'redirect_to'

    def get_context_data(self, **kwargs):
        context = super(ListView, self).get_context_data(**kwargs)
        return context


def results_view(request, pk):
    try:
        R = Calculation.objects.get(pk=pk)
    except Calculation.DoesNotExist:
        raise Http404("Calculation does not exist")

    t = loader.get_template('results_explorer/result_view.html')
    context = RequestContext(request, {})
    context.update({'Result': json.dumps(R.results_data)})
    return HttpResponse(t.template.render(context))


def result_group_view(request, pk):
    """
    Raw Display of All content in a result group
    :param request:
    :param pk:
    :return:
    """
    try:
        # Select our result group
        RG = ResultGroup.objects.get(pk=pk)
        # Get all results with foreign key to this group
        R = RG.calculation_set.all()
    except ResultGroup.DoesNotExist:
        raise Http404("Result Group does not exist")

    print(type(RG))
    print(type(R))
    result_collection = []
    # Collect and collate all results_data from each Result
    for entry in R:
        print(type(entry))
        print(entry)
        result_collection.append(entry.results_data)

    t = loader.get_template('results_explorer/result_group_view.html')
    context = RequestContext(request, {})
    context.update({'Result': json.dumps(result_collection)})
    return HttpResponse(t.template.render(context))


def result_group_logfiles(request, pk):
    """
    Raw Display of All logfiles of a result group
    :param request:
    :param pk:
    :return:
    """
    try:
        # Select our result group
        RG = ResultGroup.objects.get(pk=pk)
        # Get all results with foreign key to this group
        R = RG.calculation_set.all()
    except ResultGroup.DoesNotExist:
        raise Http404("Result Group does not exist")

    print(type(RG))
    print(type(R))
    logfile_collection = []
    # Collect and collate all results_data from each Result
    for entry in R:
        print(type(entry))
        print(entry)
        logfile_collection.append(entry.logfile)

    t = loader.get_template('results_explorer/result_group_logfiles.html')
    context = RequestContext(request, {})
    context.update({'logfile_collection': logfile_collection})
    return HttpResponse(t.template.render(context))
