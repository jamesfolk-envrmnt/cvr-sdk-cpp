/*
** Copyright (c) 2016 CognitiveVR, Inc. All rights reserved.
*/
#include "stdafx.h"
#include "transaction.h"

Transaction::Transaction(CognitiveVRAnalyticsCore* cog)
{
	cvr = cog;

	BatchedTransactions = json::array();
}

void Transaction::BeginPosition(std::string category, std::vector<float> &Position, std::shared_ptr<json> properties, std::string transaction_id)
{
	if (!cvr->WasInitSuccessful()) { cvr->log->Info("init not successful"); return; }

	if (!cvr->HasStartedSession())
	{
		// this should still be added to the batch, but not sent
		cvr->log->Warning("Transaction::Begin - Session not started!");
	}

	if (properties == nullptr)
	{
		properties = std::make_shared<json>();
	}

	std::string trans = std::string("TXN");

	double ts = cvr->GetTimestamp();

	json args = json::array();

	args.emplace_back(ts);
	args.emplace_back(ts);
	args.emplace_back(cvr->UserId);
	args.emplace_back(cvr->DeviceId);
	args.emplace_back(category);
	args.emplace_back(trans);
	args.emplace_back(cvr->config->kNetworkTimeout);
	args.emplace_back(transaction_id);
	args.emplace_back(*properties);

	json* ptrargs = &args;

	AddToBatch("datacollector_beginTransaction", ptrargs);

	json se = json();
	se["name"] = category;
	se["time"] = ts;
	se["point"] = { Position[0],Position[1] ,Position[2] };
	if (properties->size() > 0)
	{
		se["properties"] = *properties;
	}

	BatchedTransactionsSE.emplace_back(se);
}

void Transaction::UpdatePosition(std::string category, std::vector<float> &Position, std::shared_ptr<json> properties, std::string transaction_id, double progress)
{
	if (!cvr->WasInitSuccessful()) { cvr->log->Info("init not successful"); return; }

	if (!cvr->HasStartedSession())
	{
		cvr->log->Warning("Transaction::UpdatePosition - Session not started!");
	}

	if (properties == nullptr)
	{
		properties = std::make_shared<json>();
	}

	double ts = cvr->GetTimestamp();

	json args = json::array();

	args.emplace_back(ts);
	args.emplace_back(ts);
	args.emplace_back(cvr->UserId);
	args.emplace_back(cvr->DeviceId);
	args.emplace_back(category);
	args.emplace_back(progress);
	args.emplace_back(transaction_id);
	args.emplace_back(*properties);

	json* ptrargs = &args;

	AddToBatch("datacollector_updateTransaction", ptrargs);

	json se = json();
	se["name"] = category;
	se["time"] = ts;
	se["point"] = { Position[0],Position[1] ,Position[2] };
	if (properties->size() > 0)
	{
		se["properties"] = *properties;
	}

	BatchedTransactionsSE.emplace_back(se);
}

void Transaction::EndPosition(std::string category, std::vector<float> &Position, std::shared_ptr<json> properties, std::string transaction_id, std::string result)
{
	if (!cvr->WasInitSuccessful()) { cvr->log->Info("init not successful"); return; }

	if (!cvr->HasStartedSession())
	{
		cvr->log->Warning("Transaction::EndPosition - Session not started!");
	}

	if (properties == nullptr)
	{
		properties = std::make_shared<json>();
	}

	double ts = cvr->GetTimestamp();

	json args = json::array();

	args.emplace_back(ts);
	args.emplace_back(ts);
	args.emplace_back(cvr->UserId);
	args.emplace_back(cvr->DeviceId);
	args.emplace_back(category);
	args.emplace_back(result);
	args.emplace_back(transaction_id);
	args.emplace_back(*properties);

	json* ptrargs = &args;

	AddToBatch("datacollector_endTransaction", ptrargs);

	json se = json();
	se["name"] = category;
	se["time"] = ts;
	se["point"] = { Position[0],Position[1] ,Position[2] };
	if (properties->size() > 0)
	{
		se["properties"] = *properties;
	}

	BatchedTransactionsSE.emplace_back(se);
}

void Transaction::BeginEndPosition(std::string category, std::vector<float> &Position, std::shared_ptr<json> properties, std::string transaction_id, std::string result)
{
	if (!cvr->HasStartedSession())
	{
		cvr->log->Warning("Transaction::BeginEndPosition - Session not started!");
	}

	this->EndPosition(category, Position, properties, transaction_id, result);
}

void Transaction::AddToBatch(std::string method, json* args)
{
	cvr->log->Info("batch begin");
	json batchObject;

	batchObject["method"] = method;
	if (args == nullptr)
	{
		batchObject["args"] = nullptr;
	}
	else
	{
		batchObject["args"] = *args;
	}

	BatchedTransactions.emplace_back(batchObject);

	transactionCount++;
	if (transactionCount >= cvr->config->TransactionBatchSize)
	{
		SendData();
	}
}

void Transaction::SendData()
{
	if (!cvr->WasInitSuccessful()) { cvr->log->Info("init not successful"); return; }

	if (!cvr->HasStartedSession())
	{
		cvr->log->Warning("Transaction::SendData - Session not started!");
		return;
	}

	//send to dashboard
	json data = json::array();
	data.emplace_back(cvr->GetTimestamp());
	data.emplace_back(BatchedTransactions);
	cvr->network->DashboardCall("datacollector_batch", data.dump());

	BatchedTransactions.clear();

	//send to sceneexplorer
	json se = json();
	se["userid"] = cvr->UserId;
	se["timestamp"] = (int)cvr->GetTimestamp();
	se["sessionid"] = cvr->GetSessionID();
	se["part"] = jsonPart;
	jsonPart++;
	se["data"] = BatchedTransactionsSE;
	cvr->network->SceneExplorerCall("events", se.dump());

	BatchedTransactionsSE.clear();

	transactionCount = 0;
}