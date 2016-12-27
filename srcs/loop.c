/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpinchon <gpinchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/06 18:59:58 by gpinchon          #+#    #+#             */
/*   Updated: 2016/12/27 12:09:51 by gpinchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal_framework.h>

Uint8	mousemoved(void *framework)
{
	t_point2	mousepos;

	mousepos = get_mouse_pos(framework);
	return (
	((t_framework*)framework)->lastmousepos.x != mousepos.x
	|| ((t_framework*)framework)->lastmousepos.y != mousepos.y);
}

void	check_mouse(t_framework *f, SDL_Event *e)
{
	Uint8		type;
	t_point2	mousepos;

	mousepos = get_mouse_pos(f);
	if (mousemoved(f) && f->mousemove[e->button.windowID].function)
		f->mousemove[e->button.windowID].
			function(f->mousemove[e->button.windowID].arg, mousepos);
	f->lastmousepos = mousepos;
	if (e->type == SDL_MOUSEBUTTONDOWN
	|| e->type == SDL_MOUSEBUTTONUP)
	{
		type = e->type == SDL_MOUSEBUTTONDOWN;
		f->buttons[e->button.button] = type;
		if (f->mouse[e->button.windowID][e->button.button][type].function)
			f->mouse[e->button.windowID][e->button.button][type].
				function(f->mouse[e->button.windowID][e->button.button]
					[type].arg, e->button.button);
	}
}

void	check_keyboard(t_framework *f, SDL_Event *e)
{
	if (e->type == SDL_KEYDOWN)
	{
		f->keys[e->key.keysym.scancode] = 1;
		if (f->keydown[e->key.windowID][e->key.keysym.scancode][e->key.repeat].
			function)
			f->keydown[e->key.windowID][e->key.keysym.scancode][e->key.repeat].
				function(f->keydown[e->key.windowID][e->key.keysym.scancode]
					[e->key.repeat].arg, e->key.keysym.scancode);
	}
	else if (e->type == SDL_KEYUP)
	{
		f->keys[e->key.keysym.scancode] = 0;
		if (f->keyup[e->key.windowID][e->key.keysym.scancode].function)
			f->keyup[e->key.windowID][e->key.keysym.scancode].
				function(f->keyup[e->key.windowID][e->key.keysym.scancode].arg,
					e->key.keysym.scancode);
	}
}

char	framework_is_done(void *framework)
{
	return (((t_framework*)framework)->done);
}

void	framework_loop_once(void *framework)
{
	t_framework	*f;
	SDL_Event	e;

	FRAMEWORK_DEBUG(!framework,
		NULL_FRAMEWORK_POINTER, "framework_loop_once");
	f = framework;
	if (f->done)
		return ;
	if (f->loop.function)
		f->loop.function(f->loop.arg);
	if (!SDL_PollEvent(&e)
	|| (f->done = e.type == SDL_QUIT))
		return ;
	else if (e.type == SDL_APP_LOWMEMORY && f->low_mem.function)
		f->low_mem.function(f->low_mem.arg);
	else
	{
		check_mouse(framework, &e);
		check_keyboard(framework, &e);
	}
}

void	framework_loop(void *framework)
{
	SDL_Event	e;
	t_framework	*f;

	FRAMEWORK_DEBUG(!framework, NULL_FRAMEWORK_POINTER, "framework_loop");
	f = framework;
	while (!f->done)
	{
		if (f->loop.function)
			f->loop.function(f->loop.arg);
		if (SDL_PollEvent(&e))
		{
			if ((f->done = e.type == SDL_QUIT))
			{
				if (f->onexit.function)
					f->onexit.function(f->onexit.arg);
				break ;
			}
			if (e.type == SDL_APP_LOWMEMORY && f->low_mem.function)
				f->low_mem.function(f->low_mem.arg);
			check_mouse(framework, &e);
			check_keyboard(framework, &e);
		}
	}
}
