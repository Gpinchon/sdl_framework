/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpinchon <gpinchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/06 18:59:58 by gpinchon          #+#    #+#             */
/*   Updated: 2017/01/20 20:44:10 by gpinchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal_framework.h>

Uint8	mousemoved(void *framework)
{
	t_point2	mousepos;
	t_point2	lastmousepos;

	mousepos = get_mouse_pos(framework);
	lastmousepos = ((t_framework*)framework)->lastmousepos;
	return (lastmousepos.x != mousepos.x || lastmousepos.y != mousepos.y);
}

void	check_mouse(t_framework *f, SDL_Event e)
{
	Uint8		type;
	t_point2	mousepos;
	t_callback	c;

	mousepos = get_mouse_pos(f);
	c = f->mousemove[e.button.windowID];
	if (mousemoved(f) && c.function)
		c.function(f->mousemove[e.button.windowID].arg, mousepos);
	f->lastmousepos = mousepos;
	if (e.type == SDL_MOUSEBUTTONDOWN
	|| e.type == SDL_MOUSEBUTTONUP)
	{
		type = e.type == SDL_MOUSEBUTTONDOWN;
		c = f->mouse[e.button.windowID][e.button.button][type];
		f->buttons[e.button.button] = type;
		if (c.function)
			c.function(c.arg, e.button.button);
	}
}

void	check_keyboard(t_framework *f, SDL_Event e)
{
	t_callback	c;

	if (e.type == SDL_KEYDOWN)
	{
		c = f->keydown[e.key.windowID][e.key.keysym.scancode][e.key.repeat];
		f->keys[e.key.keysym.scancode] = 1;
		if (c.function)
			c.function(c.arg, e.key.keysym.scancode);
	}
	else if (e.type == SDL_KEYUP)
	{
		c = f->keyup[e.key.windowID][e.key.keysym.scancode];
		f->keys[e.key.keysym.scancode] = 0;
		if (c.function)
			c.function(c.arg, e.key.keysym.scancode);
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
		check_mouse(framework, e);
		check_keyboard(framework, e);
	}
}

void	framework_loop(void *framework)
{
	SDL_Event	e;
	t_framework	*f;
	t_callback	loop;
	t_callback	onexit;

	FRAMEWORK_DEBUG(!framework, NULL_FRAMEWORK_POINTER, "framework_loop");
	f = framework;
	while (!f->done)
	{
		loop = f->loop;
		onexit = f->onexit;
		if (loop.function)
			loop.function(loop.arg);
		if (SDL_PollEvent(&e))
		{
			if ((f->done = e.type == SDL_QUIT))
			{
				if (onexit.function)
					onexit.function(onexit.arg);
				break ;
			}
			if (e.type == SDL_APP_LOWMEMORY && f->low_mem.function)
				f->low_mem.function(f->low_mem.arg);
			check_mouse(framework, e);
			check_keyboard(framework, e);
		}
	}
}
