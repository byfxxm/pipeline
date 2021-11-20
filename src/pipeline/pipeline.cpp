#include "stdafx.h"
#include "pipeline.h"
#include "pipeline_imp.h"
#include "worker.h"

void* pipeline_create()
{
	return new pipeline_imp();
}

void pipeline_delete(void* pipeline)
{
	delete (pipeline_imp*)pipeline;
}

void pipeline_start(void* pipeline)
{
	return ((pipeline_imp*)pipeline)->start();
}

void pipeline_stop(void* pipeline)
{
	return ((pipeline_imp*)pipeline)->stop();
}

void pipeline_add_procedure(void* pipeline, procedure proc)
{
	return ((pipeline_imp*)pipeline)->add_procedure(proc);
}