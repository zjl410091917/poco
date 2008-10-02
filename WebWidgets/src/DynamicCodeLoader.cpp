//
// DynamicCodeLoader.cpp
//
// $Id: //poco/Main/WebWidgets/src/DynamicCodeLoader.cpp#2 $
//
// Library: WebWidgets
// Package: Core
// Module:  DynamicCodeLoader
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/WebWidgets/DynamicCodeLoader.h"
#include "Poco/WebWidgets/RequestHandler.h"
#include "Poco/Net/NameValueCollection.h"
#include "Poco/Net/HTTPServerResponse.h"


namespace Poco {
namespace WebWidgets {


const std::string DynamicCodeLoader::EV_LOAD("dynload");


DynamicCodeLoader::DynamicCodeLoader(View* pParent, const Poco::URI& uri, const std::string& fctName, View::Ptr pView):
	Renderable(typeid(DynamicCodeLoader)),
	_pParent(pParent),
	_uri(uri),
	_fctName(fctName),
	_loaderFct("load"),
	_loadAllFct("loadAll"),
	_pView(pView)
{
	poco_check_ptr (_pView);
	poco_assert (!_fctName.empty());
	_loaderFct.append(_fctName);
	_loadAllFct.append(_fctName);
}


DynamicCodeLoader::~DynamicCodeLoader()
{
}


void DynamicCodeLoader::handleForm(const std::string& field, const std::string& value)
{
}

	
void DynamicCodeLoader::handleAjaxRequest(const Poco::Net::NameValueCollection& args, Poco::Net::HTTPServerResponse& response)
{
	const std::string& ev = args[RequestHandler::KEY_EVID];
	if (ev == EV_LOAD)
	{
		beforeLoad(this, _pView);
		DynamicCodeLoader* pThis = this;
		generateCode(this, pThis);
		/// send the JS presentation of the page
		response.setContentType("text/javascript");
		response.setChunkedTransferEncoding(true);
		serializeJSON(response.send(),"");
	}
	else
		response.send();
}

		
bool DynamicCodeLoader::serializeJSON(std::ostream& out, const std::string& name)
{
	out << _code;
	return true;
}


} } // namespace Poco::WebWidgets
