# This file is part of the Solstice distribution (https://github.com/open-risk/solstice).
# Copyright (c) 2022 - 2024 Open Risk (https://www.openriskmanagement.com)+
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

import requests
from controller import settings
from django.http import HttpResponse
from django.template import RequestContext, loader
from django.views.generic import DetailView

from controller.serializers import WorkflowDetailSerializer
from workflow_explorer.forms import WorkflowJSONForm
from workflow_explorer.models import Workflow, WorkflowJSON

cgi_url = settings.CGI_URL
cgi_path = settings.CGI_PATH


def workflow_post(request, pk):
    """
    Debugging workflows

    """

    # Load existing workflow data
    workflow_json = WorkflowJSON.objects.get(pk=pk)
    if request.method == 'POST':
        form = WorkflowJSONForm(request.POST)
        name = form['name'].value()
        payload = form['payload'].value()

        workflow_data = payload
        print("POST PAYLOAD", workflow_data)

    else:

        form = WorkflowJSONForm(instance=workflow_json)
        workflow_data = json.dumps(workflow_json.payload)
        print("GET PAYLOAD", workflow_data)


    # Post calculation ticket directly
    model_url = cgi_url
    # construct header
    # print("MODEL_URL", model_url)
    headers = {'Content-Type': 'application/json'}
    response = requests.post(model_url, data=workflow_data, headers=headers, timeout=None, verify=False)
    print("RESPONSE STATUS", response.status_code)
    # print(dir(response))
    print("RESPONSE TEXT", response.text)
    # rj = response.json()
    # print("RESPONSE JSON", rj)

    results_data = response.text
    log_data = ""

    t = loader.get_template('workflow_explorer/workflow_post.html')
    context = RequestContext(request, {})
    context.update({'object': workflow_json, 'form': form})
    context.update({'results_data': results_data})
    context.update({'log_data': log_data})
    return HttpResponse(t.template.render(context))